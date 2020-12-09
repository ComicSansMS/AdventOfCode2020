#include <encoding_error.hpp>

#include <fmt/printf.h>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>

#include <algorithm>
#include <bitset>
#include <cassert>
#include <cctype>
#include <limits>
#include <numeric>
#include <regex>
#include <sstream>
#include <string>


std::vector<int64_t> parseInput(std::string_view input)
{
    return ranges::to<std::vector<int64_t>>(input | ranges::views::split('\n') |
                                            ranges::views::transform([](auto sv) -> int64_t {
                                                return std::stoll(ranges::to<std::string>(sv));
                                            }));
}

bool checkEncoding(std::unordered_set<int64_t> const& preamble, int64_t candidate)
{
    for (auto const& first : preamble) {
        int64_t const second = candidate - first;
        auto it_second = preamble.find(second);
        if ((it_second != end(preamble)) && (*it_second != first)) {
            return true;
        }
    }
    return false;
}

int64_t findInvalid(std::vector<int64_t> const& numbers, std::size_t preamble_length)
{
    assert(preamble_length > 0);
    std::unordered_set<int64_t> preamble;
    for (std::size_t i = 0; i < numbers.size(); ++i) {
        if (i >= preamble_length) {
            assert(preamble.size() == preamble_length);

            int64_t const candidate = numbers[i];
            if (!checkEncoding(preamble, candidate)) { return candidate; }
            preamble.erase(numbers[i - preamble_length]);
        }
        preamble.insert(numbers[i]);
    }
    return -1;
}

std::vector<int64_t> findWeakRange(std::vector<int64_t> const& numbers, int64_t invalid_number)
{
    for (std::size_t i = 0; i < numbers.size(); ++i)
    {
        int64_t acc = numbers[i];
        std::size_t i_end = i + 1;
        while (acc < invalid_number) {
            acc += numbers[i_end];
            ++i_end;
        }
        if (acc == invalid_number) {
            return std::vector<int64_t>(numbers.begin() + i, numbers.begin() + i_end);
        }
    }
    return {};
}

int64_t breakEncryption(std::vector<int64_t> const& numbers, std::size_t preamble_length)
{
    int64_t const invalid_number = findInvalid(numbers, preamble_length);
    auto weak_range = findWeakRange(numbers, invalid_number);
    std::sort(begin(weak_range), end(weak_range));
    return weak_range.front() + weak_range.back();
}
