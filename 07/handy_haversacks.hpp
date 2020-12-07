#ifndef ADVENT_OF_CODE_07_HANDY_HAVERSACKS_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_07_HANDY_HAVERSACKS_HPP_INCLUDE_GUARD

#include <array>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

struct Color {
    enum class Attribute {
        light,
        dark,
        bright,
        muted,
        shiny,
        vibrant,
        faded,
        dotted,
        clear,
        dim,
        mirrored,
        posh,
        plaid,
        drab,
        striped,
        pale,
        dull,
        wavy,
    } attribute;
    
    enum class Hue {
        red,
        orange,
        white,
        yellow,
        gold,
        olive,
        plum,
        blue,
        black,
        crimson,
        violet,
        tomato,
        salmon,
        lime,
        fuchsia,
        indigo,
        beige,
        maroon,
        cyan,
        brown,
        green,
        purple,
        magenta,
        teal,
        silver,
        bronze,
        coral,
        gray,
        aqua,
        turquoise,
        lavender,
        tan,
        chartreuse,
    } hue;

    friend bool operator==(Color const& lhs, Color const& rhs) = default;
};

namespace std
{
template<> struct hash<Color>
{
    std::size_t operator()(Color const& c) const noexcept
    {
        return static_cast<std::size_t>(c.attribute) * 100 + static_cast<std::size_t>(c.hue);
    }
};
}

struct Edge {
    int amount;
    std::size_t sourceNodeIndex;
    std::size_t destinationNodeIndex;
};

struct Graph {
    std::vector<Color> nodes;
    std::vector<std::vector<Edge>> edges;
};

Graph parseInput(std::string_view input);

int countContainersFor(Graph const& g, Color c);

int countBagsContainedIn(Graph const& g, Color c);

#endif
