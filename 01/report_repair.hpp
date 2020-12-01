#ifndef ADVENT_OF_CODE_01_REPORT_REPAIR_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_01_REPORT_REPAIR_HPP_INCLUDE_GUARD

#include <string_view>
#include <tuple>
#include <vector>

std::vector<int> parseInput(std::string_view input);

std::tuple<int, int> findSum(std::vector<int> const& values, int sum);

int expenseReport1(std::vector<int> const& values);

std::tuple<int, int, int> findSum3(std::vector<int> const& values, int sum);

int expenseReport2(std::vector<int> const& values);

#endif
