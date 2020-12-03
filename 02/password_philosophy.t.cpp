#include <password_philosophy.hpp>

#include <catch.hpp>

TEST_CASE("Password Philosophy")
{
    char const sample_input[] = "1-3 a: abcde\n1-3 b: cdefg\n2-9 c: ccccccccc\n";
    auto const v = parseInput(sample_input);

    SECTION("Parse Input")
    {
        REQUIRE(v.size() == 3);
        CHECK(v[0].min == 1);
        CHECK(v[0].max == 3);
        CHECK(v[0].c == 'a');
        CHECK(v[0].password == "abcde");

        CHECK(v[1].min == 1);
        CHECK(v[1].max == 3);
        CHECK(v[1].c == 'b');
        CHECK(v[1].password == "cdefg");

        CHECK(v[2].min == 2);
        CHECK(v[2].max == 9);
        CHECK(v[2].c == 'c');
        CHECK(v[2].password == "ccccccccc");
    }

    SECTION("Check Password")
    {
        CHECK(checkPassword(v[0]));
        CHECK(!checkPassword(v[1]));
        CHECK(checkPassword(v[2]));
    }

    SECTION("Count Valid Passwords")
    {
        CHECK(countValidPasswords(v) == 2);
    }

    SECTION("Check Password2")
    {
        CHECK(checkPassword2(v[0]));
        CHECK(!checkPassword2(v[1]));
        CHECK(!checkPassword2(v[2]));
    }

    SECTION("Count Valid Passwords2")
    {
        CHECK(countValidPasswords2(v) == 1);
    }
}
