#include <monster_messages.hpp>

#include <fmt/printf.h>

#include <algorithm>
#include <bitset>
#include <cassert>
#include <cctype>
#include <charconv>
#include <iterator>
#include <limits>
#include <numeric>
#include <regex>
#include <sstream>
#include <string>

RulesAndWords parseInput(std::string_view input)
{
    RulesAndWords ret;
    std::regex rx_line(R"(((\d+): (.+))|(\w+)[\r\n])");
    std::regex rx_terminal(R"x("(\w)")x");
    std::regex rx_reference(R"((\d+)( (\d+))?( (\d+))?)");
    std::regex rx_refpair(R"((\d+)( (\d+))? \| (\d+)( (\d+))?)");

    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(begin(input), end(input), rx_line);
    auto const it_end = regex_it();

    bool parsing_rules = true;
    for (auto it = it_begin; it != it_end; ++it) {
        auto const& smatch = *it;
        if (smatch[1].matched) {
            assert(parsing_rules);
            std::size_t index;
            std::from_chars(&(*smatch[2].first), &(*smatch[2].second), index);
            if (ret.rules.size() <= index) { ret.rules.resize(index + 1, Rule{Empty{}}); }

            std::match_results<std::string_view::iterator> sub_smatch;
            if (std::regex_match(smatch[3].first, smatch[3].second, sub_smatch, rx_terminal)) {
                assert(std::distance(sub_smatch[1].first, sub_smatch[1].second) == 1);
                Terminal t;
                t.c = *sub_smatch[1].first;
                ret.rules[index] = t;
            } else if(std::regex_match(smatch[3].first, smatch[3].second, sub_smatch, rx_reference)) {
                Reference r;
                std::size_t tmp;
                std::from_chars(&(*sub_smatch[1].first), &(*sub_smatch[1].second), tmp);
                r.push_back(tmp);
                if (sub_smatch[2].matched) {
                    std::from_chars(&(*sub_smatch[3].first), &(*sub_smatch[3].second), tmp);
                    r.push_back(tmp);
                }
                if (sub_smatch[4].matched) {
                    std::from_chars(&(*sub_smatch[5].first), &(*sub_smatch[5].second), tmp);
                    r.push_back(tmp);
                }
                ret.rules[index] = r;
            } else if(std::regex_match(smatch[3].first, smatch[3].second, sub_smatch, rx_refpair)) {
                Alternatives alts;
                std::size_t tmp;
                Reference r1;
                std::from_chars(&(*sub_smatch[1].first), &(*sub_smatch[1].second), tmp);
                r1.push_back(tmp);
                if (sub_smatch[2].matched) {
                    std::from_chars(&(*sub_smatch[3].first), &(*sub_smatch[3].second), tmp);
                    r1.push_back(tmp);
                }
                Reference r2;
                std::from_chars(&(*sub_smatch[4].first), &(*sub_smatch[4].second), tmp);
                r2.push_back(tmp);
                if (sub_smatch[5].matched) {
                    std::from_chars(&(*sub_smatch[6].first), &(*sub_smatch[6].second), tmp);
                    r2.push_back(tmp);
                }
                alts.push_back(std::move(r1));
                alts.push_back(std::move(r2));
                ret.rules[index] = std::move(alts);
            } else {
                fmt::print("Parsing error rule #{}: '{}'\n", index, smatch[3].str());
            }
        } else {
            parsing_rules = false;
            ret.words.push_back(smatch[4].str());
        }
    }
    return ret;
}

Match matchRule(std::vector<Rule> const&, std::string const&, std::size_t, Empty const&)
{
    fmt::print("Trying to match empty rule.\n");
    return {};
}

Match matchRule(std::vector<Rule> const&, std::string const& word, std::size_t offset, Terminal const& terminal)
{
    Match m;
    if (word[offset] == terminal.c) {
        return Match{ offset, offset + 1 };
    } else {
        return Match{};
    }
}

Match matchRule(std::vector<Rule> const& rules, std::string const& word, std::size_t offset, Reference const& refer)
{
    std::size_t current_offset = offset;
    for (auto const& r : refer) {
        Rule const& rule = rules[r];
        auto const match = std::visit([&rules, &word, current_offset](auto&& r) -> Match { return matchRule(rules, word, current_offset, r); }, rule);
        if (!match.does_match) { return Match{}; }
        current_offset = match.match_end;
    }
    return Match{ offset, current_offset };
}

Match matchRule(std::vector<Rule> const& rules, std::string const& word, std::size_t offset, Alternatives const& alts)
{
    std::size_t current_offset = offset;
    for (auto const& a : alts) {
        Reference const& r = a;
        auto const match = matchRule(rules, word, current_offset, r);
        if (match.does_match) { return match; }
    }
    return Match{};
}

bool match(std::vector<Rule> const& rules, std::size_t rule_index, std::string const& word)
{
    Rule const& rule = rules[rule_index];
    Match const match = std::visit([&rules, &word](auto&& r) -> Match { return matchRule(rules, word, 0, r); }, rule);
    return (match.does_match && (match.match_start == 0) && (match.match_end == word.size()));
}

bool match(std::vector<Rule> const& rules, std::string const& word)
{
    return match(rules, 0, word);
}

int64_t countMatchingWords(RulesAndWords const& rules_and_words)
{
    auto const& [rules, words] = rules_and_words;
    return std::count_if(begin(words), end(words), [&rules](std::string const& w) { return match(rules, w); });
}
