#include <operation_order.hpp>

#include <fmt/format.h>

#include <catch.hpp>

TEST_CASE("Operation Order")
{
    char const sample_line1[] = "1 + 2 * 3 + 4 * 5 + 6";

    SECTION("Lex Input")
    {
        auto const tokens = lexLine("12 + 2123 * (3 + 48)");
        CHECK(tokens.size() == 9);
        CHECK(tokens[0].type == Token::Type::Terminal);
        CHECK(tokens[0].token == "12");
        CHECK(tokens[1].type == Token::Type::Operator);
        CHECK(tokens[1].token == "+");
        CHECK(tokens[2].type == Token::Type::Terminal);
        CHECK(tokens[2].token == "2123");
        CHECK(tokens[3].type == Token::Type::Operator);
        CHECK(tokens[3].token == "*");
        CHECK(tokens[4].type == Token::Type::ParenOpen);
        CHECK(tokens[4].token == "(");
        CHECK(tokens[5].type == Token::Type::Terminal);
        CHECK(tokens[5].token == "3");
        CHECK(tokens[6].type == Token::Type::Operator);
        CHECK(tokens[6].token == "+");
        CHECK(tokens[7].type == Token::Type::Terminal);
        CHECK(tokens[7].token == "48");
        CHECK(tokens[8].type == Token::Type::ParenClose);
        CHECK(tokens[8].token == ")");

        auto const tokens2 = lexLine("1 + 2");
        REQUIRE(tokens2.size() == 3);
        CHECK(tokens2[0].type == Token::Type::Terminal);
        CHECK(tokens2[0].token == "1");
        CHECK(tokens2[1].type == Token::Type::Operator);
        CHECK(tokens2[1].token == "+");
        CHECK(tokens2[2].type == Token::Type::Terminal);
        CHECK(tokens2[2].token == "2");

    }

    SECTION("Parse Input")
    {
        {
            auto expr = parseLine("1 + 2");
            REQUIRE(expr.getType() == Nodes::Type::Addition);
            auto node_add = expr.getAs<Nodes::Addition>();
            REQUIRE(node_add->lhs.getType() == Nodes::Type::Terminal);
            REQUIRE(node_add->rhs.getType() == Nodes::Type::Terminal);
            CHECK(node_add->lhs.getAs<Nodes::Terminal>()->value == 1);
            CHECK(node_add->rhs.getAs<Nodes::Terminal>()->value == 2);
        }
        {
            auto expr = parseLine("1 + 2 + 3");
            REQUIRE(expr.getType() == Nodes::Type::Addition);
            auto node_add = expr.getAs<Nodes::Addition>();
            REQUIRE(node_add->lhs.getType() == Nodes::Type::Addition);
            REQUIRE(node_add->rhs.getType() == Nodes::Type::Terminal);
            auto node_add_nested = node_add->lhs.getAs<Nodes::Addition>();
            REQUIRE(node_add_nested->lhs.getType() == Nodes::Type::Terminal);
            REQUIRE(node_add_nested->rhs.getType() == Nodes::Type::Terminal);
            CHECK(node_add_nested->lhs.getAs<Nodes::Terminal>()->value == 1);
            CHECK(node_add_nested->rhs.getAs<Nodes::Terminal>()->value == 2);
            CHECK(node_add->rhs.getAs<Nodes::Terminal>()->value == 3);
        }
        {
            auto expr = parseLine("1 + (2 * 5)");
            REQUIRE(expr.getType() == Nodes::Type::Addition);
            auto node_add = expr.getAs<Nodes::Addition>();
            REQUIRE(node_add->lhs.getType() == Nodes::Type::Terminal);
            REQUIRE(node_add->rhs.getType() == Nodes::Type::Multiplication);
            CHECK(node_add->lhs.getAs<Nodes::Terminal>()->value == 1);
            auto node_mul = node_add->rhs.getAs<Nodes::Multiplication>();
            CHECK(node_mul->lhs.getType() == Nodes::Type::Terminal);
            CHECK(node_mul->rhs.getType() == Nodes::Type::Terminal);
            CHECK(node_mul->lhs.getAs<Nodes::Terminal>()->value == 2);
            CHECK(node_mul->rhs.getAs<Nodes::Terminal>()->value == 5);
        }
        {
            CHECK(parseLine("1 + (2 * 5)").toString() == "(1 + (2 * 5))");
            CHECK(parseLine("1 + 2 * 5").toString() == "((1 + 2) * 5)");
            CHECK(parseLine("1 + (2 * 3) + (4 * (5 + 6))").toString() == "((1 + (2 * 3)) + (4 * (5 + 6)))");
            CHECK(parseLine("5 + (8 * 3 + 9)").toString() == "(5 + ((8 * 3) + 9))");
            
            CHECK(parseLine("((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2").toString() ==
                "(((((((2 + 4) * 9) * (((6 + 9) * 8) + 6)) + 6) + 2) + 4) * 2)");
        }
    }

    SECTION("Evaluate")
    {
        CHECK(parseLine("42").evaluate() == 42);
        CHECK(parseLine("1 + 2").evaluate() == 3);
        CHECK(parseLine("1 + 2 * 3 + 4 * 5 + 6").evaluate() == 71);
        CHECK(parseLine("2 * 3 + (4 * 5)").evaluate() == 26);
        CHECK(parseLine("5 + (8 * 3 + 9 + 3 * 4 * 3)").evaluate() == 437);
        CHECK(parseLine("5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))").evaluate() == 12240);
        CHECK(parseLine("((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2").evaluate() == 13632);
    }

    SECTION("Evaluate And Sum")
    {
        auto const nodes = parseInput("42\n1 + 2 * 3 + 4 * 5 + 6\n5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))\n((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2\n");
        CHECK(evaluateAndSum(nodes) == 25985);
    }

    SECTION("Change Operation Order Precedence")
    {
        CHECK(parseLine("1 * 2 + 3").toString() == "((1 * 2) + 3)");
        CHECK(changeAdditionPrecedence(parseLine("1 * 2 + 3")).toString() == "(1 * (2 + 3))");

        CHECK(changeAdditionPrecedence(parseLine("1 + (2 * 3) + (4 * (5 + 6))")).evaluate() == 51);
        CHECK(changeAdditionPrecedence(parseLine("2 * 3 + (4 * 5)")).evaluate() == 46);
        CHECK(changeAdditionPrecedence(parseLine("5 + (8 * 3 + 9 + 3 * 4 * 3)")).evaluate() == 1445);
        CHECK(changeAdditionPrecedence(parseLine("5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))")).evaluate() == 669060);
        CHECK(changeAdditionPrecedence(parseLine("((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2")).evaluate() == 23340);
    }

}
