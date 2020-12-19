#include <monster_messages.hpp>

#include <fmt/format.h>

#include <catch.hpp>

TEST_CASE("Monster Messages")
{
    char const sample_input1[] = R"(0: 1 2
1: "a"
2: 1 3 | 3 1
3: "b"
)";
    char const sample_input2[] = R"(0: 4 1 5
4: "a"
2: 4 4 | 5 5
3: 4 5 | 5 4
5: "b"
1: 2 3 | 3 2

ababbb
bababa
abbbab
aaabbb
aaaabbb
)";

    SECTION("Parse Input")
    {
        {
            auto const rules_and_words = parseInput(sample_input1);
            auto const& rules = rules_and_words.rules;
            REQUIRE(rules.size() == 4);
            REQUIRE(rules[0].index() == static_cast<std::size_t>(RuleIndex::Reference));
            CHECK(std::get<Reference>(rules[0]) == Reference{ 1, 2 });
            REQUIRE(rules[1].index() == static_cast<std::size_t>(RuleIndex::Terminal));
            CHECK(std::get<Terminal>(rules[1]).c == 'a');
            REQUIRE(rules[2].index() == static_cast<std::size_t>(RuleIndex::Alternatives));
            CHECK(std::get<Alternatives>(rules[2]) == Alternatives{ Reference{ 1, 3 }, Reference{ 3, 1 } });
            REQUIRE(rules[3].index() == static_cast<std::size_t>(RuleIndex::Terminal));
            CHECK(std::get<Terminal>(rules[3]).c == 'b');
            auto const& words = rules_and_words.words;
            CHECK(words.empty());
        }
        {
            auto const rules_and_words = parseInput(sample_input2);
            auto const& rules = rules_and_words.rules;
            CHECK(std::get<Reference>(rules[0]) == Reference{ 4, 1, 5 });
            CHECK(std::get<Alternatives>(rules[1]) == Alternatives{ Reference{ 2, 3 }, Reference{ 3, 2 } });
            CHECK(std::get<Alternatives>(rules[2]) == Alternatives{ Reference{ 4, 4 }, Reference{ 5, 5 } });
            CHECK(std::get<Alternatives>(rules[3]) == Alternatives{ Reference{ 4, 5 }, Reference{ 5, 4 } });
            CHECK(std::get<Terminal>(rules[4]).c == 'a');
            CHECK(std::get<Terminal>(rules[5]).c == 'b');
            auto const& words = rules_and_words.words;
            CHECK(words == std::vector<std::string>{ "ababbb", "bababa", "abbbab", "aaabbb", "aaaabbb" });
        }
        {
            auto special_rules = parseInput("116: 36 | 99\n8: 42\n").rules;
            REQUIRE(special_rules.size() == 117);
            for (std::size_t i = 0; i < special_rules.size(); ++i) {
                if ((i != 8) && (i != 116)) {
                    CHECK(special_rules[i].index() == static_cast<std::size_t>(RuleIndex::Empty));
                }
            }
            CHECK(std::get<Alternatives>(special_rules[116]) == Alternatives{ Reference{ 36 }, Reference{ 99 } });
            CHECK(std::get<Reference>(special_rules[8]) == Reference{ 42 });
        }
    }

    SECTION("Rule Match Empty")
    {
        auto rules_and_words = parseInput(sample_input1);
        auto& rules = rules_and_words.rules;
        rules.push_back(Empty{});

        CHECK(matchRule(rules, "", 0, std::get<Empty>(rules.back())) == Match{});
    }

    SECTION("Rule Match Terminal")
    {
        auto rules_and_words = parseInput(sample_input1);
        auto const& rules = rules_and_words.rules;

        CHECK(matchRule(rules, "a", 0, std::get<Terminal>(rules[1])) == Match{ 0, 1 });
        CHECK(matchRule(rules, "a", 0, std::get<Terminal>(rules[3])) == Match{});
        CHECK(matchRule(rules, "ab", 0, std::get<Terminal>(rules[1])) == Match{ 0, 1 });
        CHECK(matchRule(rules, "ab", 1, std::get<Terminal>(rules[1])) == Match{});
        CHECK(matchRule(rules, "ab", 1, std::get<Terminal>(rules[3])) == Match{ 1, 2 });
    }

    SECTION("Rule Match Reference")
    {
        {
            auto rules_and_words = parseInput("0: 1 2\n1: \"a\"\n2: \"b\"\n");
            auto const& rules = rules_and_words.rules;

            CHECK(matchRule(rules, "ab", 0, std::get<Reference>(rules[0])) == Match{ 0, 2 });
            CHECK(matchRule(rules, "aa", 0, std::get<Reference>(rules[0])) == Match{});
            CHECK(matchRule(rules, "ba", 0, std::get<Reference>(rules[0])) == Match{});
            CHECK(matchRule(rules, "bb", 0, std::get<Reference>(rules[0])) == Match{});
            CHECK(matchRule(rules, "aabb", 0, std::get<Reference>(rules[0])) == Match{});
            CHECK(matchRule(rules, "aabb", 1, std::get<Reference>(rules[0])) == Match{ 1, 3 });
            CHECK(matchRule(rules, "aabb", 2, std::get<Reference>(rules[0])) == Match{});
        }
        {
            auto rules_and_words = parseInput("0: 3 4\n3: 1 1\n4: 2 1\n1: \"a\"\n2: \"b\"\n");
            auto const& rules = rules_and_words.rules;

            CHECK(matchRule(rules, "aaba", 0, std::get<Reference>(rules[0])) == Match{ 0, 4 });
            CHECK(matchRule(rules, "aabb", 0, std::get<Reference>(rules[0])) == Match{});
            CHECK(matchRule(rules, "aaabaa", 1, std::get<Reference>(rules[0])) == Match{ 1, 5 });
            CHECK(matchRule(rules, "aaabaa", 2, std::get<Reference>(rules[0])) == Match{});
        }
    }

    SECTION("Rule Match Alternatives")
    {
        auto rules_and_words = parseInput(sample_input1);
        auto const& rules = rules_and_words.rules;

        CHECK(matchRule(rules, "aab", 0, std::get<Reference>(rules[0])) == Match{ 0, 3 });
        CHECK(matchRule(rules, "aba", 0, std::get<Reference>(rules[0])) == Match{ 0, 3 });
        CHECK(matchRule(rules, "abb", 0, std::get<Reference>(rules[0])) == Match{});
        CHECK(matchRule(rules, "aaa", 0, std::get<Reference>(rules[0])) == Match{});
        CHECK(matchRule(rules, "baa", 0, std::get<Reference>(rules[0])) == Match{});
    }

    SECTION("Match Words")
    {
        auto rules_and_words = parseInput(sample_input2);
        auto const& rules = rules_and_words.rules;
        auto const& words = rules_and_words.words;

        CHECK(match(rules, words[0]));
        CHECK(!match(rules, words[1]));
        CHECK(match(rules, words[2]));
        CHECK(!match(rules, words[3]));
        CHECK(!match(rules, words[4]));
    }

    SECTION("Count Matching Words")
    {
        CHECK(countMatchingWords(parseInput(sample_input2)) == 2);
    }

    SECTION("Generate")
    {
        CHECK(generateFromRule(parseInput(sample_input1).rules, 1) == std::vector<std::string>{ "a" });
        CHECK(generateFromRule(parseInput(sample_input1).rules, 3) == std::vector<std::string>{ "b" });
        CHECK(generateFromRule(parseInput(sample_input1).rules, 2) == std::vector<std::string>{ "ab", "ba" });
        CHECK(generateFromRule(parseInput(sample_input1).rules, 0) == std::vector<std::string>{ "aab", "aba" });

        CHECK(generateFromRule(parseInput(sample_input2).rules, 0) ==
            std::vector<std::string>{ "aaaabb", "aaabab", "abbabb", "abbbab", "aabaab", "aabbbb", "abaaab", "ababbb" });
    }

    char const sample_input3[] = R"(42: 9 14 | 10 1
9: 14 27 | 1 26
10: 23 14 | 28 1
1: "a"
11: 42 31
5: 1 14 | 15 1
19: 14 1 | 14 14
12: 24 14 | 19 1
16: 15 1 | 14 14
31: 14 17 | 1 13
6: 14 14 | 1 14
2: 1 24 | 14 4
0: 8 11
13: 14 3 | 1 12
15: 1 | 14
17: 14 2 | 1 7
23: 25 1 | 22 14
28: 16 1
4: 1 1
20: 14 14 | 1 15
3: 5 14 | 16 1
27: 1 6 | 14 18
14: "b"
21: 14 1 | 1 14
25: 1 1 | 1 14
22: 14 14
8: 42
26: 14 22 | 1 20
18: 15 15
7: 14 5 | 1 21
24: 14 1

abbbbbabbbaaaababbaabbbbabababbbabbbbbbabaaaa
bbabbbbaabaabba
babbbbaabbbbbabbbbbbaabaaabaaa
aaabbbbbbaaaabaababaabababbabaaabbababababaaa
bbbbbbbaaaabbbbaaabbabaaa
bbbababbbbaaaaaaaabbababaaababaabab
ababaaaaaabaaab
ababaaaaabbbaba
baabbaaaabbaaaababbaababb
abbbbabbbbaaaababbbbbbaaaababb
aaaaabbaabaaaaababaa
aaaabbaaaabbaaa
aaaabbaabbaaaaaaabbbabbbaaabbaabaaa
babaaabbbaaabaababbaabababaaab
aabbbbbaabbbaaaaaabbbbbababaaaaabbaaabba
)";

    SECTION("Rules With Loops")
    {
        auto rules_and_words = parseInput(sample_input3);
        CHECK(countMatchingWords(rules_and_words) == 3);
        CHECK(countWithLoops(rules_and_words) == 12);
    }
}
