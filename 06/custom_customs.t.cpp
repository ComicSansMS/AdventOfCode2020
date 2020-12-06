#include <custom_customs.hpp>

#include <catch.hpp>

TEST_CASE("Custom Customs")
{
    char const sample_input[] = "abc\n\na\nb\nc\n\nab\nac\n\na\na\na\na\n\nb";
    auto const all_answers = parseInput(sample_input);

    SECTION("Parse Input")
    {
        REQUIRE(all_answers.size() == 5);

        auto const& group1 = all_answers[0];
        REQUIRE(group1.answers.size() == 1);
        CHECK(group1.answers[0][0] == true);
        CHECK(group1.answers[0][1] == true);
        CHECK(group1.answers[0][2] == true);
        CHECK(std::all_of(group1.answers[0].begin() + 3, std::end(group1.answers[0]),
                          [](bool b) { return b == false; }));
        CHECK(group1.group_answers[0] == true);
        CHECK(group1.group_answers[1] == true);
        CHECK(group1.group_answers[2] == true);
        CHECK(std::all_of(group1.group_answers.begin() + 3, std::end(group1.group_answers),
                          [](bool b) { return b == false; }));

        auto const& group2 = all_answers[1];
        REQUIRE(group2.answers.size() == 3);
        CHECK(group2.answers[0][0] == true);
        CHECK(group2.answers[0][1] == false);
        CHECK(group2.answers[0][2] == false);
        CHECK(std::all_of(group2.answers[0].begin() + 3, std::end(group2.answers[0]),
                          [](bool b) { return b == false; }));
        CHECK(group2.answers[1][0] == false);
        CHECK(group2.answers[1][1] == true);
        CHECK(group2.answers[1][2] == false);
        CHECK(std::all_of(group2.answers[1].begin() + 3, std::end(group2.answers[1]),
                          [](bool b) { return b == false; }));
        CHECK(group2.answers[2][0] == false);
        CHECK(group2.answers[2][1] == false);
        CHECK(group2.answers[2][2] == true);
        CHECK(std::all_of(group2.answers[2].begin() + 3, std::end(group2.answers[2]),
                          [](bool b) { return b == false; }));
        CHECK(group2.group_answers[0] == true);
        CHECK(group2.group_answers[1] == true);
        CHECK(group2.group_answers[2] == true);
        CHECK(std::all_of(group2.group_answers.begin() + 3, std::end(group2.group_answers),
                          [](bool b) { return b == false; }));

        auto const& group3 = all_answers[2];
        REQUIRE(group3.answers.size() == 2);
        CHECK(group3.answers[0][0] == true);
        CHECK(group3.answers[0][1] == true);
        CHECK(group3.answers[0][2] == false);
        CHECK(std::all_of(group3.answers[0].begin() + 3, std::end(group3.answers[0]),
                          [](bool b) { return b == false; }));
        CHECK(group3.answers[1][0] == true);
        CHECK(group3.answers[1][1] == false);
        CHECK(group3.answers[1][2] == true);
        CHECK(std::all_of(group3.answers[1].begin() + 3, std::end(group3.answers[1]),
                          [](bool b) { return b == false; }));
        CHECK(group3.group_answers[0] == true);
        CHECK(group3.group_answers[1] == true);
        CHECK(group3.group_answers[2] == true);
        CHECK(std::all_of(group3.group_answers.begin() + 3, std::end(group3.group_answers),
                          [](bool b) { return b == false; }));

        auto const& group4 = all_answers[3];
        REQUIRE(group4.answers.size() == 4);
        CHECK(group4.answers[0][0] == true);
        CHECK(std::all_of(group4.answers[0].begin() + 1, std::end(group4.answers[0]),
                          [](bool b) { return b == false; }));
        CHECK(group4.answers[1][0] == true);
        CHECK(std::all_of(group4.answers[1].begin() + 1, std::end(group4.answers[1]),
                          [](bool b) { return b == false; }));
        CHECK(group4.answers[2][0] == true);
        CHECK(std::all_of(group4.answers[2].begin() + 1, std::end(group4.answers[2]),
                          [](bool b) { return b == false; }));
        CHECK(group4.answers[3][0] == true);
        CHECK(std::all_of(group4.answers[3].begin() + 1, std::end(group4.answers[3]),
                          [](bool b) { return b == false; }));
        CHECK(group4.group_answers[0] == true);
        CHECK(std::all_of(group4.group_answers.begin() + 1, std::end(group4.group_answers),
                          [](bool b) { return b == false; }));

        auto const& group5 = all_answers[4];
        REQUIRE(group5.answers.size() == 1);
        CHECK(group5.answers[0][0] == false);
        CHECK(group5.answers[0][1] == true);
        CHECK(std::all_of(group5.answers[0].begin() + 2, std::end(group5.answers[0]),
                          [](bool b) { return b == false; }));
        CHECK(group5.group_answers[0] == false);
        CHECK(group5.group_answers[1] == true);
        CHECK(std::all_of(group5.group_answers.begin() + 2, std::end(group5.group_answers),
                          [](bool b) { return b == false; }));
    }

    SECTION("Count Yes Answers")
    {
        CHECK(countYesAnswers(all_answers[0].answers[0]) == 3);
        CHECK(countYesAnswers(all_answers[0].group_answers) == 3);

        CHECK(countYesAnswers(all_answers[1].answers[0]) == 1);
        CHECK(countYesAnswers(all_answers[1].answers[1]) == 1);
        CHECK(countYesAnswers(all_answers[1].answers[2]) == 1);
        CHECK(countYesAnswers(all_answers[1].group_answers) == 3);

        CHECK(countYesAnswers(all_answers[2].answers[0]) == 2);
        CHECK(countYesAnswers(all_answers[2].answers[1]) == 2);
        CHECK(countYesAnswers(all_answers[2].group_answers) == 3);

        CHECK(countYesAnswers(all_answers[3].answers[0]) == 1);
        CHECK(countYesAnswers(all_answers[3].answers[1]) == 1);
        CHECK(countYesAnswers(all_answers[3].answers[2]) == 1);
        CHECK(countYesAnswers(all_answers[3].answers[3]) == 1);
        CHECK(countYesAnswers(all_answers[3].group_answers) == 1);

        CHECK(countYesAnswers(all_answers[4].answers[0]) == 1);
        CHECK(countYesAnswers(all_answers[4].group_answers) == 1);
    }

    SECTION("Sum of Group Yes Answers")
    {
        CHECK(sumOfGroupYesAnswers(all_answers) == 11);
    }

    SECTION("Count All Yes Answers In Group")
    {
        CHECK(countAllYesAnswersInGroup(all_answers[0]) == 3);
        CHECK(countAllYesAnswersInGroup(all_answers[1]) == 0);
        CHECK(countAllYesAnswersInGroup(all_answers[2]) == 1);
        CHECK(countAllYesAnswersInGroup(all_answers[3]) == 1);
        CHECK(countAllYesAnswersInGroup(all_answers[4]) == 1);
    }

    SECTION("Count All Yes Answers")
    {
        CHECK(countAllYesAnswers(all_answers) == 6);
    }
}
