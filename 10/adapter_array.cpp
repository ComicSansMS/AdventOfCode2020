#include <adapter_array.hpp>

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


std::vector<int> parseInput(std::string_view input)
{
    return ranges::to<std::vector<int>>(input | ranges::views::split('\n') |
                                        ranges::views::transform([](auto sv) -> int {
                                            return std::stoi(ranges::to<std::string>(sv));
                                        }));
}

AdapterChain chainAdapters(std::vector<int> const& adapters)
{
    assert(!adapters.empty());
    std::vector<int> chain = adapters;
    std::sort(begin(chain), end(chain));
    AdapterChain ret;
    ret.builtin_joltage = chain.back() + 3;
    std::vector<int> diffs;
    diffs.reserve(chain.size());
    std::adjacent_difference(begin(chain), end(chain), std::back_inserter(diffs));
    ret.jolt_diff_1 = static_cast<int>(std::count(begin(diffs), end(diffs), 1));
    ret.jolt_diff_3 = static_cast<int>(std::count(begin(diffs), end(diffs), 3)) + 1;
    assert(ret.jolt_diff_1 + ret.jolt_diff_3 == chain.size() + 1);
    return ret;
}

int64_t countValidChains(std::vector<int> const& adapters)
{
    std::vector<int> chain = adapters;
    std::sort(begin(chain), end(chain));

    std::vector<int> diffs;
    diffs.reserve(chain.size());
    std::adjacent_difference(begin(chain), end(chain), std::back_inserter(diffs));

    std::vector<int64_t> valid_chains;
    valid_chains.resize(diffs.size());
    for (std::size_t i = 0; i < valid_chains.size(); ++i) {
        if (diffs[i] == 3) {
            valid_chains[i] = (i > 0) ? valid_chains[i - 1] : 1;
        } else {
            assert(diffs[i] == 1);
            int64_t const valid_paths_from_pred = (i > 0) ? valid_chains[i - 1] : 1;
            valid_chains[i] += valid_paths_from_pred;
            if (i >= 1) {
                int const diff_pred_pred = diffs[i] + diffs[i - 1];
                if (diff_pred_pred <= 3) {
                    int64_t const valid_paths_from_pred_pred = (i > 1) ? valid_chains[i - 2] : 1;
                    valid_chains[i] += valid_paths_from_pred_pred;
                }
                if (i >= 2) {
                    int const diff_pred_pred_pred = diff_pred_pred + diffs[i - 2];
                    if (diff_pred_pred_pred <= 3) {
                        int64_t const valid_paths_from_pred_pred = (i > 2) ? valid_chains[i - 3] : 1;
                        valid_chains[i] += valid_paths_from_pred_pred;
                    }
                }
            }
            
        }
    }

    return valid_chains.back();
}
