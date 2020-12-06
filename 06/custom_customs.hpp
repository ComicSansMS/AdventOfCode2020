#ifndef ADVENT_OF_CODE_06_CUSTOM_CUSTOMS_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_06_CUSTOM_CUSTOMS_HPP_INCLUDE_GUARD

#include <array>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

using Answers = std::array<bool, 26>;
struct Group {
    std::vector<Answers> answers;
    Answers group_answers;
};

std::vector<Group> parseInput(std::string_view input);

int countYesAnswers(Answers const& a);

int sumOfGroupYesAnswers(std::vector<Group> const& g);

int countAllYesAnswersInGroup(Group const& g);

int countAllYesAnswers(std::vector<Group> const& g);

#endif
