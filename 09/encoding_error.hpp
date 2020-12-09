#ifndef ADVENT_OF_CODE_09_ECNDODING_ERROR_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_09_ECNDODING_ERROR_HPP_INCLUDE_GUARD

#include <array>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_set>
#include <variant>
#include <vector>

std::vector<int64_t> parseInput(std::string_view input);

bool checkEncoding(std::unordered_set<int64_t> const& preamble, int64_t candidate);

int64_t findInvalid(std::vector<int64_t> const& numbers, std::size_t preamble_length);

std::vector<int64_t> findWeakRange(std::vector<int64_t> const& numbers, int64_t invalid_number);

int64_t breakEncryption(std::vector<int64_t> const& numbers, std::size_t preamble_length);

#endif
