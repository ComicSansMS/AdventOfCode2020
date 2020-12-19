#ifndef ADVENT_OF_CODE_19_MONSTER_MESSAGES_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_19_MONSTER_MESSAGES_HPP_INCLUDE_GUARD

#include <fmt/format.h>

#include <array>
#include <bitset>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

struct Empty {};

struct Terminal {
    char c;
};

using Reference = std::vector<std::size_t>;

using Alternatives = std::vector<Reference>;

using Rule = std::variant<Empty, Terminal, Reference, Alternatives>;

enum class RuleIndex : std::size_t {
    Empty = 0,
    Terminal,
    Reference,
    Alternatives
};

struct RulesAndWords {
    std::vector<Rule> rules;
    std::vector<std::string> words;
};

RulesAndWords parseInput(std::string_view input);

struct Match {
    bool does_match;
    std::size_t match_start;
    std::size_t match_end;

    Match()
        :does_match(false), match_start(0), match_end(0)
    {}

    Match(std::size_t start, std::size_t end)
        :does_match(true), match_start(start), match_end(end)
    {}

    friend bool operator==(Match const&, Match const&) = default;
};

Match matchRule(std::vector<Rule> const& rules, std::string const& word, std::size_t offset, Empty const&);
Match matchRule(std::vector<Rule> const& rules, std::string const& word, std::size_t offset, Terminal const&);
Match matchRule(std::vector<Rule> const& rules, std::string const& word, std::size_t offset, Reference const&);
Match matchRule(std::vector<Rule> const& rules, std::string const& word, std::size_t offset, Alternatives const&);

bool match(std::vector<Rule> const& rules, std::size_t rule_index, std::string const& word);

bool match(std::vector<Rule> const& rules, std::string const& word);

int64_t countMatchingWords(RulesAndWords const& rules_and_words);

#endif
