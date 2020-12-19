#include <operation_order.hpp>

#include <fmt/printf.h>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>

#include <algorithm>
#include <bitset>
#include <cassert>
#include <cctype>
#include <charconv>
#include <iterator>
#include <limits>
#include <numeric>
#include <regex>
#include <sstream>
#include <string>

namespace Nodes {
std::string toString(Invalid const&)
{
    return "Invalid";
}
std::string toString(Terminal const& n)
{
    return std::to_string(n.value);
}

std::string toString(Addition const& n)
{
    return "(" + n.lhs.toString() + " + " + n.rhs.toString() + ")";
}

std::string toString(Multiplication const& n)
{
    return "(" + n.lhs.toString() + " * " + n.rhs.toString() + ")";
}

int64_t evaluate(Invalid const&)
{
    assert(false);
    return 0;
}

int64_t evaluate(Terminal const& n)
{
    return n.value;
}

int64_t evaluate(Addition const& n)
{
    return n.lhs.evaluate() + n.rhs.evaluate();
}

int64_t evaluate(Multiplication const& n)
{
    return n.lhs.evaluate() * n.rhs.evaluate();
}
}

Node::NodeConcept::~NodeConcept() = default;

std::vector<Token> lexLine(std::string_view input)
{
    std::vector<Token> ret;
    std::string_view::iterator token_begin = input.begin();
    auto const addTerminal = [&ret, &token_begin, input](std::string_view::iterator it) {
        if (it == token_begin) { return; }
        std::string_view token_str = input.substr(std::distance(input.begin(), token_begin),
                                                  std::distance(token_begin, it));
        assert(std::all_of(begin(token_str), end(token_str),
                           [](char c) { return (c >= '0') && (c <= '9'); }));
        ret.push_back(Token{ Token::Type::Terminal, token_str });
    };
    for (auto it = input.begin(), it_end = input.end(); it != it_end; ++it) {
        auto const& c = *it;
        if (((c < '0') || (c > '9'))) {
            addTerminal(it);
            token_begin = it + 1;

            if ((c == ' ') || (c == '\n')) { continue; }
            std::string_view const token_str = input.substr(std::distance(input.begin(), it), 1);
            if (c == '(') {
                ret.push_back(Token{ Token::Type::ParenOpen, token_str });
            } else if (c == ')') {
                ret.push_back(Token{ Token::Type::ParenClose, token_str });
            } else if ((c == '+') || (c == '*')) {
                ret.push_back(Token{ Token::Type::Operator, token_str });
            } else {
                fmt::print("Unexpected token: '{}'", token_str);
                return {};
            }
        }
    }
    addTerminal(input.end());
    return ret;
}

Nodes::Terminal parseTerminal(Token const& t)
{
    assert(t.type == Token::Type::Terminal);
    Nodes::Terminal ret;
    auto const res = std::from_chars(t.token.data(), t.token.data() + t.token.size(), ret.value);
    assert(res.ec == std::errc{});
    return ret;
}

Node parseNode_rec(std::vector<Token>::const_iterator& it, std::vector<Token>::const_iterator it_end, int nested_level);

Node parseSubexpression(std::vector<Token>::const_iterator& it, std::vector<Token>::const_iterator it_end, int nested_level)
{
    Token const& t1 = *it++;
    if (t1.type == Token::Type::ParenOpen) {
        return parseNode_rec(it, it_end, nested_level + 1);
    } else if (t1.type == Token::Type::Terminal) {
        return Node(parseTerminal(t1), nested_level);
    } else {
        fmt::print("Unexpected token (type {}).", t1.type);
        return {};
    }
}

std::tuple<Node, bool> parseNodeOpAndRhs(std::vector<Token>::const_iterator& it, std::vector<Token>::const_iterator it_end, Node&& lhs, int nested_level)
{
    Token const& t_op = *it++;
    if (t_op.type == Token::Type::ParenClose) { return std::make_tuple(lhs, true); }
    if (t_op.type != Token::Type::Operator) {
        fmt::print("Unexpected token (type {}, expected {}).", t_op.type, Token::Type::Operator);
        return {};
    }

    Node const rhs = parseSubexpression(it, it_end, nested_level);
    if (t_op.token == "+") {
        Nodes::Addition ret;
        ret.lhs = std::move(lhs);
        ret.rhs = std::move(rhs);
        return std::make_tuple(Node(ret, nested_level), false);
    } else if(t_op.token == "*") {
        Nodes::Multiplication ret;
        ret.lhs = std::move(lhs);
        ret.rhs = std::move(rhs);
        return std::make_tuple(Node(ret, nested_level), false);
    } else {
        fmt::print("Unexpected operator {}.", t_op.token);
    }
    return {};
}

Node parseNode_rec(std::vector<Token>::const_iterator& it, std::vector<Token>::const_iterator it_end, int nested_level)
{
    Node lhs = parseSubexpression(it, it_end, nested_level);
    if (it == it_end) {
        assert(nested_level == 0);
        return lhs;
    }
    while (it != it_end) {
        auto const ret = parseNodeOpAndRhs(it, it_end, std::move(lhs), nested_level);
        lhs = std::get<0>(ret);
        if (std::get<1>(ret)) { break; }
    }
    return lhs;
}


Node parseLine(std::string_view line)
{
    auto const tokens = lexLine(line);
    auto it = tokens.begin();
    Node ret = parseNode_rec(it, tokens.end(), 0);
    return ret;
}

std::vector<Node> parseInput(std::string_view input)
{
    std::vector<Node> ret;
    for (auto const& sv : input | ranges::views::split('\n')) {
        std::string_view strv(&*sv.begin(), ranges::distance(sv));
        ret.push_back(parseLine(strv));
    }
    return ret;
}

int64_t evaluateAndSum(std::vector<Node> const& nodes)
{
     int64_t acc = 0;
     for (auto const& n : nodes) {
         acc += n.evaluate();
     }
     return acc;
}

Node changeAdditionPrecedence(Node const& in)
{
    Node n = in;
    if (n.getType() == Nodes::Type::Addition) {
        Nodes::Addition* n_add = n.getAs<Nodes::Addition>();
        n_add->lhs = changeAdditionPrecedence(n_add->lhs);
        n_add->rhs = changeAdditionPrecedence(n_add->rhs);
        if ((n_add->lhs.getType() == Nodes::Type::Multiplication) && (n.getLevel() == n_add->lhs.getLevel())) {
            // rotate
            Node n_child = std::move(n_add->lhs);
            Nodes::Multiplication* n_child_mul = n_child.getAs<Nodes::Multiplication>();
            Node a = std::move(n_child_mul->lhs);
            Node b = std::move(n_child_mul->rhs);
            Node c = std::move(n_add->rhs);
            n_add->lhs = std::move(b);
            n_add->rhs = std::move(c);
            n_child_mul->lhs = std::move(a);
            n_child_mul->rhs = std::move(n);
            n = std::move(n_child);
        }
    } else if (n.getType() == Nodes::Type::Multiplication) {
        Nodes::Multiplication* n_mul = n.getAs<Nodes::Multiplication>();
        n_mul->lhs = changeAdditionPrecedence(n_mul->lhs);
        n_mul->rhs = changeAdditionPrecedence(n_mul->rhs);
    }
    return n;
}
