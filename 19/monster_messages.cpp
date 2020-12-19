#include <monster_messages.hpp>

#include <fmt/printf.h>

#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/range/conversion.hpp>

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

std::vector<std::string> generate(std::vector<Rule> const&, Empty const&)
{
    fmt::print("Generating from empty rule.\n");
    return {};
}

std::vector<std::string> generate(std::vector<Rule> const&, Terminal const& term)
{
    return { std::string{ term.c } };
}

std::vector<std::string> cartesian_product(std::vector<std::string> const& v1, std::vector<std::string> const& v2)
{
    std::vector<std::string> ret;
    ret.reserve(v1.size() * v2.size());
    for (auto const& l : v1) {
        for (auto const& r : v2) {
            ret.push_back(l + r);
        }
    }
    return ret;
}

std::vector<std::string> generate(std::vector<Rule> const& rules, Reference const& refs)
{
    std::vector<std::vector<std::string>> generated;
    generated.reserve(refs.size());
    for (auto const& r : refs) {
        auto const rule = rules[r];
        generated.push_back(std::visit([&rules](auto&& r) { return generate(rules, r); }, rule));
    }

    std::vector<std::string> ret = generated.front();
    for (std::size_t i = 1; i < generated.size(); ++i) {
        ret = cartesian_product(ret, generated[i]);
    }
    return ret;
}

std::vector<std::string> generate(std::vector<Rule> const& rules, Alternatives const& alts)
{
    std::vector<std::string> ret;
    for(auto const& refs : alts) {
        auto const t = generate(rules, refs);
        ret.reserve(ret.size() + t.size());
        std::move(begin(t), end(t), std::back_inserter(ret));
    }
    return ret;
}

std::vector<std::string> generateFromRule(std::vector<Rule> const& rules, std::size_t rule_index)
{
    Rule const& rule = rules[rule_index];
    auto const ret = std::visit([&rules](auto&& r) { return generate(rules, r); }, rule);
    for (auto const& w : ret) {
        assert(match(rules, rule_index, w));
    }
    return ret;
}

bool match_rule_loop(std::vector<std::string> const& loop42, std::vector<std::string> const& loop31, std::string const& w)
{
    std::size_t const chunk_size42 = loop42.front().size();
    std::size_t i = 0;
    int loop_count_42 = 0;
    for (; i < w.size(); i += chunk_size42) {
        std::string chunk_i = w.substr(i, chunk_size42);
        if (std::find(begin(loop42), end(loop42), chunk_i) == end(loop42)) { break; }
        ++loop_count_42;
    }
    if (i == w.size()) { return false; }
    std::size_t const chunk_size31 = loop31.front().size();
    int loop_count_31 = 0;
    for (; i < w.size(); i += chunk_size31) {
        std::string chunk_i = w.substr(i, chunk_size31);
        if (std::find(begin(loop31), end(loop31), chunk_i) == end(loop31)) { return false; }
        ++loop_count_31;
    }
    return (loop_count_42 > loop_count_31);
}

int64_t countWithLoops(RulesAndWords const& rules_and_words)
{
    int64_t ret = 0;
    std::vector<std::string> matches;
    auto const& [rules, words] = rules_and_words;
    auto const loop42 = generateFromRule(rules, 42);
    assert(std::all_of(begin(loop42), end(loop42),
           [s = loop42.front().size()](std::string const& str) { return str.size() == s; }));
    auto const loop31 = generateFromRule(rules, 31);
    assert(std::all_of(begin(loop31), end(loop31),
           [s = loop31.front().size()](std::string const& str) { return str.size() == s; }));
    for (auto const& w : words) {
        bool const m1 = match(rules, w);
        bool const m2 = match_rule_loop(loop42, loop31, w);
        if (m1 || m2) {
            ++ret;
            matches.push_back(w);
        }
    }
    return ret;
}

/*

bbabbbbaabaabba
babbbbaabbbbbabbbbbbaabaaabaaa
aaabbbbbbaaaabaababaabababbabaaabbababababaaa
bbbbbbbaaaabbbbaaabbabaaa
bbbababbbbaaaaaaaabbababaaababaabab
ababaaaaaabaaab
ababaaaaabbbaba
baabbaaaabbaaaababbaababb
abbbbabbbbaaaababbbbbbaaaababb
 aaaaabbaabaaaaababaa
"aaaaabbaabaaaaababaa"

"aaaabbaaaabbaaa"

 aaaabbaabbaaaaaaabbbabbbaaabbaabaaa
"aaaabbaabbaaaaaaabbbabbbaaabbaabaaa"

 aabbbbbaabbbaaaaaabbbbbababaaaaabbaaabba
"aabbbbbaabbbaaaaaabbbbbababaaaaabbaaabba"


*/
