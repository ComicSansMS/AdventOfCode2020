#ifndef ADVENT_OF_CODE_18_OPERATION_ORDER_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_18_OPERATION_ORDER_HPP_INCLUDE_GUARD

#include <fmt/format.h>

#include <array>
#include <bitset>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

namespace Nodes {
enum class Type {
    Invalid,
    Addition,
    Multiplication,
    Terminal
};

struct Invalid {};
struct Terminal;
struct Addition;
struct Multiplication;

template<typename T>
struct IsNode : public std::false_type {};

template<> struct IsNode<Terminal> : public std::true_type {};
template<> struct IsNode<Addition> : public std::true_type {};
template<> struct IsNode<Multiplication> : public std::true_type {};

inline Type getType(Invalid const&) { return Type::Invalid; }
inline Type getType(Terminal const&) { return Type::Terminal; }
inline Type getType(Addition const&) { return Type::Addition; }
inline Type getType(Multiplication const&) { return Type::Multiplication; }

std::string toString(Invalid const& n);
std::string toString(Terminal const& n);
std::string toString(Addition const& n);
std::string toString(Multiplication const& n);

int64_t evaluate(Invalid const& n);
int64_t evaluate(Terminal const& n);
int64_t evaluate(Addition const& n);
int64_t evaluate(Multiplication const& n);
}

class Node {
private:
    class NodeConcept {
    public:
        virtual ~NodeConcept() = 0;
        virtual std::unique_ptr<NodeConcept> clone() const = 0;
        virtual Nodes::Type getType() const = 0;
        virtual std::string toString() const = 0;
        virtual int64_t evaluate() const = 0;
    };

    template<typename T>
    class NodeModel : public NodeConcept {
    private:
        T m_node;
    public:
        explicit NodeModel(T n)
            :m_node(std::move(n))
        {}

        ~NodeModel() override = default;

        std::unique_ptr<NodeConcept> clone() const override {
            return std::make_unique<NodeModel<T>>(*this);
        }

        Nodes::Type getType() const override {
            return Nodes::getType(m_node);
        }

        std::string toString() const override {
            return Nodes::toString(m_node);
        }

        int64_t evaluate() const override {
            return Nodes::evaluate(m_node);
        }

        T* getNode() {
            return &m_node;
        }
    };
private:
    std::unique_ptr<NodeConcept> m_node;
    int m_level;
public:
    Node()
        :m_node(std::make_unique<NodeModel<Nodes::Invalid>>(Nodes::Invalid{})), m_level(-1)
    {}

    template<typename T, typename = std::enable_if_t<Nodes::IsNode<std::decay_t<T>>::value>>
    Node(T&& t, int node_level)
        :m_node(std::make_unique<NodeModel<std::decay_t<T>>>(std::forward<T>(t))), m_level(node_level)
    {}

    Node(Node const& rhs)
        :m_node(rhs.m_node->clone()), m_level(rhs.m_level)
    {}

    Node(Node&& rhs)
        :m_node(std::move(rhs.m_node)), m_level(rhs.m_level)
    {}

    Node& operator=(Node const& rhs)
    {
        if (this != &rhs) {
            m_node = rhs.m_node->clone();
            m_level = rhs.m_level;
        }
        return *this;
    }

    Node& operator=(Node&& rhs) {
        if (this != &rhs) {
            std::swap(m_node, rhs.m_node);
            m_level = rhs.m_level;
        }
        return *this;
    }

    Nodes::Type getType() const {
        return m_node->getType();
    }

    std::string toString() const {
        return m_node->toString();
    }

    int64_t evaluate() const {
        return m_node->evaluate();
    }

    template<typename T>
    T* getAs() {
        NodeModel<T>* ptr = dynamic_cast<NodeModel<T>*>(m_node.get());
        return (ptr) ? ptr->getNode() : nullptr;
    }

    template<typename T>
    T const* getAs() const {
        NodeModel<T>* ptr = dynamic_cast<NodeModel<T>*>(m_node.get());
        return (ptr) ? ptr->getNode() : nullptr;
    }

    int getLevel() const {
        return m_level;
    }
};

namespace Nodes {
struct Terminal {
    int64_t value;
};

struct Addition {
    Node lhs;
    Node rhs;
};

struct Multiplication {
    Node lhs;
    Node rhs;
};

}

struct Token {
    enum class Type {
        ParenOpen,
        ParenClose,
        Operator,
        Terminal
    } type;
    std::string_view token;
};

template<>
struct fmt::formatter<Node>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(Node const& node, FormatContext& ctx) {
        return fmt::format_to(ctx.out(), "{}", node.toString());
    }
};

std::vector<Token> lexLine(std::string_view input);

Node parseLine(std::string_view line);

std::vector<Node> parseInput(std::string_view input);

int64_t evaluateAndSum(std::vector<Node> const& nodes);

Node changeAdditionPrecedence(Node const& in);

#endif
