#include <encoding_error.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/iota.hpp>

#include <catch.hpp>

TEST_CASE("Handheld Halting")
{
    char const sample_input[] = 
        R"(35
20
15
25
47
40
62
55
65
95
102
117
150
182
127
219
299
277
309
576)";

    SECTION("Parse Input")
    {
        auto example = parseInput("1\n2\n82\n123456789\n99999999999999");
        REQUIRE(example.size() == 5);
        CHECK(example == std::vector<int64_t>{ 1, 2, 82, 123456789, 99999999999999 });
    }

    SECTION("Check Encoding")
    {
        std::unordered_set<int64_t> preamble;
        for (auto const& i : ranges::views::iota(static_cast<int64_t>(1), static_cast<int64_t>(26))) {
            preamble.insert(i);
        }

        CHECK(checkEncoding(preamble, 26));
        CHECK(checkEncoding(preamble, 49));
        CHECK(!checkEncoding(preamble, 100));
        CHECK(!checkEncoding(preamble, 50));
    }

    SECTION("Find Invalids")
    {
        auto const numbers = parseInput(sample_input);
        CHECK(findInvalid(numbers, 5) == 127);
    }

    SECTION("Find Weak Range")
    {
        auto const numbers = parseInput(sample_input);
        auto const weak_range = findWeakRange(numbers, findInvalid(numbers, 5));
        CHECK(weak_range == std::vector<int64_t>{ 15, 25, 47, 40 });
    }

    SECTION("Break Encryption")
    {
        auto const numbers = parseInput(sample_input);
        CHECK(breakEncryption(numbers, 5) == 62);
    }
}
