#include <handy_haversacks.hpp>

#include <fmt/printf.h>

#include <algorithm>
#include <bitset>
#include <cassert>
#include <cctype>
#include <limits>
#include <numeric>
#include <regex>
#include <sstream>
#include <string>

Color::Attribute parseAttribute(std::string_view input)
{
    if (input == "light") {
        return Color::Attribute::light;
    } else if (input == "dark") {
        return Color::Attribute::dark;
    } else if (input == "bright") {
        return Color::Attribute::bright;
    } else if (input == "muted") {
        return Color::Attribute::muted;
    } else if (input == "shiny") {
        return Color::Attribute::shiny;
    } else if (input == "vibrant") {
        return Color::Attribute::vibrant;
    } else if (input == "faded") {
        return Color::Attribute::faded;
    } else if (input == "clear") {
        return Color::Attribute::clear;
    } else if (input == "dim") {
        return Color::Attribute::dim;
    } else if (input == "mirrored") {
        return Color::Attribute::mirrored;
    } else if (input == "posh") {
        return Color::Attribute::posh;
    } else if (input == "plaid") {
        return Color::Attribute::plaid;
    } else if (input == "drab") {
        return Color::Attribute::drab;
    } else if (input == "striped") {
        return Color::Attribute::striped;
    } else if (input == "pale") {
        return Color::Attribute::pale;
    } else if (input == "dull") {
        return Color::Attribute::dull;
    } else if (input == "wavy") {
        return Color::Attribute::wavy;
    } else {
        assert(input == "dotted");
        return Color::Attribute::dotted;
    }
}

Color::Hue parseHue(std::string_view input)
{
    if (input == "red") {
        return Color::Hue::red;
    } else if (input == "orange") {
        return Color::Hue::orange;
    } else if (input == "white") {
        return Color::Hue::white;
    } else if (input == "yellow") {
        return Color::Hue::yellow;
    } else if (input == "gold") {
        return Color::Hue::gold;
    } else if (input == "olive") {
        return Color::Hue::olive;
    } else if (input == "plum") {
        return Color::Hue::plum;
    } else if (input == "blue") {
        return Color::Hue::blue;
    } else if (input == "crimson") {
        return Color::Hue::crimson;
    } else if (input == "violet") {
        return Color::Hue::violet;
    } else if (input == "tomato") {
        return Color::Hue::tomato;
    } else if (input == "salmon") {
        return Color::Hue::salmon;
    } else if (input == "lime") {
        return Color::Hue::lime;
    } else if (input == "fuchsia") {
        return Color::Hue::fuchsia;
    } else if (input == "indigo") {
        return Color::Hue::indigo;
    } else if (input == "beige") {
        return Color::Hue::beige;
    } else if (input == "maroon") {
        return Color::Hue::maroon;
    } else if (input == "cyan") {
        return Color::Hue::cyan;
    } else if (input == "brown") {
        return Color::Hue::brown;
    } else if (input == "green") {
        return Color::Hue::green;
    } else if (input == "purple") {
        return Color::Hue::purple;
    } else if (input == "magenta") {
        return Color::Hue::magenta;
    } else if (input == "teal") {
        return Color::Hue::teal;
    } else if (input == "silver") {
        return Color::Hue::silver;
    } else if (input == "bronze") {
        return Color::Hue::bronze;
    } else if (input == "coral") {
        return Color::Hue::coral;
    } else if (input == "gray") {
        return Color::Hue::gray;
    } else if (input == "aqua") {
        return Color::Hue::aqua;
    } else if (input == "turquoise") {
        return Color::Hue::turquoise;
    } else if (input == "lavender") {
        return Color::Hue::lavender;
    } else if (input == "tan") {
        return Color::Hue::tan;
    } else if (input == "chartreuse") {
        return Color::Hue::chartreuse;
    } else {
        assert(input == "black");
        return Color::Hue::black;
    }
}

Color parseColor(std::string_view input_attribute, std::string_view input_hue)
{
    Color ret;
    ret.attribute = parseAttribute(input_attribute);
    ret.hue = parseHue(input_hue);
    return ret;
}

struct RawEdge {
    int amount;
    Color destination;
};

std::vector<RawEdge> parseSubclause(std::string_view input)
{
    std::regex rx_subexp(R"((\d+) (\w+) (\w+) bag(s?)[\,\.])");

    std::vector<RawEdge> ret;

    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(begin(input), end(input), rx_subexp);
    auto const it_end = regex_it();
    for (auto it = it_begin; it != it_end; ++it)
    {
        auto const smatch = *it;
        auto const amount = std::stoi(smatch[1].str());
        auto const dest_color = parseColor(std::string_view(smatch[2].first, smatch[2].second),
                                           std::string_view(smatch[3].first, smatch[3].second));
        ret.push_back(RawEdge{ amount, dest_color });
    }
    return ret;
}

Graph parseInput(std::string_view input)
{
    Graph ret;

    std::regex rx_line(R"((\w+) (\w+) bags contain (.*)[\r\n]?)");

    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(begin(input), end(input), rx_line);
    auto const it_end = regex_it();

    std::vector<std::vector<RawEdge>> raw_edges;
    for (auto it = it_begin; it != it_end; ++it)
    {
        auto const smatch = *it;
        auto const source_color = parseColor(std::string_view(smatch[1].first, smatch[1].second),
                                             std::string_view(smatch[2].first, smatch[2].second));

        assert(std::find(begin(ret.nodes), end(ret.nodes), source_color) == end(ret.nodes));
        ret.nodes.push_back(source_color);

        auto const contents_str = std::string_view(smatch[3].first, smatch[3].second);
        if (contents_str != "no other bags.") {
            raw_edges.emplace_back(parseSubclause(contents_str));
        } else {
            raw_edges.emplace_back();
        }
    }

    for (std::size_t i = 0; i < ret.nodes.size(); ++i) {
        ret.edges.emplace_back();
        for (auto const& [amount, dest_color] : raw_edges[i]) {
            Edge e;
            e.amount = amount;
            e.sourceNodeIndex = i;
            e.destinationNodeIndex = std::distance(begin(ret.nodes),
                                                   std::find(begin(ret.nodes), end(ret.nodes), dest_color));
            ret.edges.back().push_back(e);
        }
    }

    return ret;
}

int countContainersFor(Graph const& g, Color c)
{
    auto const target_index = std::distance(begin(g.nodes), std::find(begin(g.nodes), end(g.nodes), c));
    std::vector<std::size_t> containers;
    containers.push_back(target_index);
    std::size_t start_index = 0;
    while (start_index != containers.size()) {
        for (auto const& es: g.edges) {
            for (auto const& e: es) {
                if (e.destinationNodeIndex == containers[start_index]) {
                    if (std::find(begin(containers), end(containers), e.sourceNodeIndex) == end(containers)) {
                        containers.push_back(e.sourceNodeIndex);
                    }
                }
            }
        }
        ++start_index;
    }
    return static_cast<int>(containers.size() - 1);
}

int countBagsContainedIn_Rec(Graph const& g, std::unordered_map<Color, int>& memoize, Color c)
{
    auto it = memoize.find(c);
    if (it == end(memoize)) {
        auto const target_index = std::distance(begin(g.nodes), std::find(begin(g.nodes), end(g.nodes), c));
        auto const& target_edges = g.edges[target_index];
        int acc = 0;
        for (auto const& e : target_edges) {
            int const bags_in_edge = e.amount * (1 + countBagsContainedIn_Rec(g, memoize, g.nodes[e.destinationNodeIndex]));
            acc += bags_in_edge;
        }
        auto const [new_it, was_inserted] = memoize.insert(std::make_pair(c, acc));
        assert(was_inserted);
        it = new_it;
    }
    return it->second;
}

int countBagsContainedIn(Graph const& g, Color c)
{
    std::unordered_map<Color, int> memoize;
    return countBagsContainedIn_Rec(g, memoize, c);
}
