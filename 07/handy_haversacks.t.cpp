#include <handy_haversacks.hpp>

#include <catch.hpp>

TEST_CASE("Handy Haversacks")
{
    char const sample_input[] = 
        R"(light red bags contain 1 bright white bag, 2 muted yellow bags.
dark orange bags contain 3 bright white bags, 4 muted yellow bags.
bright white bags contain 1 shiny gold bag.
muted yellow bags contain 2 shiny gold bags, 9 faded blue bags.
shiny gold bags contain 1 dark olive bag, 2 vibrant plum bags.
dark olive bags contain 3 faded blue bags, 4 dotted black bags.
vibrant plum bags contain 5 faded blue bags, 6 dotted black bags.
faded blue bags contain no other bags.
dotted black bags contain no other bags.)";

    auto const graph = parseInput(sample_input);

    SECTION("Parse Input")
    {
        REQUIRE(graph.nodes.size() == 9);
        REQUIRE(graph.edges.size() == 9);

        CHECK(graph.nodes[0] == Color{ Color::Attribute::light, Color::Hue::red });
        CHECK(graph.nodes[2] == Color{ Color::Attribute::bright, Color::Hue::white });
        CHECK(graph.nodes[3] == Color{ Color::Attribute::muted, Color::Hue::yellow });

        REQUIRE(graph.edges[0].size() == 2);
        CHECK(graph.edges[0][0].amount == 1);
        CHECK(graph.edges[0][0].sourceNodeIndex == 0);
        CHECK(graph.edges[0][0].destinationNodeIndex == 2);
        CHECK(graph.edges[0][1].amount == 2);
        CHECK(graph.edges[0][1].sourceNodeIndex == 0);
        CHECK(graph.edges[0][1].destinationNodeIndex == 3);

        CHECK(graph.nodes[1] == Color{ Color::Attribute::dark, Color::Hue::orange });
        REQUIRE(graph.edges[1].size() == 2);
        CHECK(graph.edges[1][0].amount == 3);
        CHECK(graph.edges[1][0].sourceNodeIndex == 1);
        CHECK(graph.edges[1][0].destinationNodeIndex == 2);
        CHECK(graph.edges[1][1].amount == 4);
        CHECK(graph.edges[1][1].sourceNodeIndex == 1);
        CHECK(graph.edges[1][1].destinationNodeIndex == 3);

        CHECK(graph.nodes[4] == Color{ Color::Attribute::shiny, Color::Hue::gold });
        REQUIRE(graph.edges[2].size() == 1);
        CHECK(graph.edges[2][0].amount == 1);
        CHECK(graph.edges[2][0].sourceNodeIndex == 2);
        CHECK(graph.edges[2][0].destinationNodeIndex == 4);

        CHECK(graph.nodes[8] == Color{ Color::Attribute::dotted, Color::Hue::black });
        CHECK(graph.edges[8].empty());
    }

    SECTION("Count Containers For")
    {
        CHECK(countContainersFor(graph, Color{ Color::Attribute::shiny, Color::Hue::gold }) == 4);
    }

    char const sample_input2[] = R"(shiny gold bags contain 2 dark red bags.
dark red bags contain 2 dark orange bags.
dark orange bags contain 2 dark yellow bags.
dark yellow bags contain 2 dark green bags.
dark green bags contain 2 dark blue bags.
dark blue bags contain 2 dark violet bags.
dark violet bags contain no other bags.)";

    SECTION("Count Bags Contained In")
    {
        CHECK(countBagsContainedIn(graph, Color{ Color::Attribute::faded, Color::Hue::blue }) == 0);
        CHECK(countBagsContainedIn(graph, Color{ Color::Attribute::dotted, Color::Hue::black }) == 0);
        CHECK(countBagsContainedIn(graph, Color{ Color::Attribute::vibrant, Color::Hue::plum }) == 11);
        CHECK(countBagsContainedIn(graph, Color{ Color::Attribute::dark, Color::Hue::olive }) == 7);
        CHECK(countBagsContainedIn(graph, Color{ Color::Attribute::shiny, Color::Hue::gold }) == 32);

        auto const graph2 = parseInput(sample_input2);
        CHECK(countBagsContainedIn(graph2, Color{ Color::Attribute::shiny, Color::Hue::gold }) == 126);
    }
}
