#include <report_repair.hpp>

#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/range/conversion.hpp>

#include <string>

std::vector<int> parseInput(std::string_view input)
{
    return ranges::to<std::vector<int>>(input | ranges::views::split('\n') |
        ranges::views::transform([](auto sv) {
            return std::stoi(ranges::to<std::string>(sv));
        }));
}

std::tuple<int, int> findSum(std::vector<int> const& values, int sum)
{
    for(auto [v1, v2] : ranges::views::cartesian_product(values, values))
    {
        if (v1 + v2 == sum) {
            return std::make_tuple(v1, v2);
        }
    }
    return {};
}

int expenseReport1(std::vector<int> const& values)
{
    auto const [v1, v2] = findSum(values, 2020);
    return v1 * v2;
}

std::tuple<int, int, int> findSum3(std::vector<int> const& values, int sum)
{
    for(auto [v1, v2, v3] : ranges::views::cartesian_product(values, values, values))
    {
        if (v1 + v2 + v3 == sum) {
            return std::make_tuple(v1, v2, v3);
        }
    }
    return {};
}

int expenseReport2(std::vector<int> const& values)
{
    auto const [v1, v2, v3] = findSum3(values, 2020);
    return v1 * v2 * v3;
}
