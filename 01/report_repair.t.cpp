#include <report_repair.hpp>

#include <catch.hpp>

TEST_CASE("Report Repair")
{
    char const sample_input[] = "1721\n979\n366\n299\n675\n1456\n";
    SECTION("Parse Input")
    {
        CHECK(parseInput("1\n2\n42") == std::vector<int>{1, 2, 42});
        CHECK(parseInput(sample_input) == std::vector<int>{1721, 979, 366, 299, 675, 1456});
    }

    std::vector<int> const values = parseInput(sample_input);

    SECTION("Find Sum")
    {
        auto const [v1, v2] = findSum(values, 2020);
        CHECK(v1 == 1721);
        CHECK(v2 == 299);
    }

    SECTION("Expense Report 1")
    {
        CHECK(expenseReport1(values) == 514579);
    }

    SECTION("Find Sum3")
    {
        auto const [v1, v2, v3] = findSum3(values, 2020);
        CHECK(v1 == 979);
        CHECK(v2 == 366);
        CHECK(v3 == 675);
    }

    SECTION("Expense Report 2")
    {
        CHECK(expenseReport2(values) == 241861950);
    }
}
