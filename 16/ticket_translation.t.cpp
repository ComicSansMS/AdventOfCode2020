#include <ticket_translation.hpp>

#include <fmt/format.h>

#include <catch.hpp>

TEST_CASE("Ticket Translation")
{
    char const sample_input[] = R"(class: 1-3 or 5-7
row: 6-11 or 33-44
seat: 13-40 or 45-50

your ticket:
7,1,14

nearby tickets:
7,3,47
40,4,50
55,2,20
38,6,12
)";

    auto const ticket_info = parseInput(sample_input);

    SECTION("Parse Input")
    {
        REQUIRE(ticket_info.field_descriptors.size() == 3);
        CHECK(ticket_info.field_descriptors[0].name == "class");
        CHECK(ticket_info.field_descriptors[0].r1.low == 1);
        CHECK(ticket_info.field_descriptors[0].r1.high == 3);
        CHECK(ticket_info.field_descriptors[0].r2.low == 5);
        CHECK(ticket_info.field_descriptors[0].r2.high == 7);
        CHECK(ticket_info.field_descriptors[1].name == "row");
        CHECK(ticket_info.field_descriptors[1].r1.low == 6);
        CHECK(ticket_info.field_descriptors[1].r1.high == 11);
        CHECK(ticket_info.field_descriptors[1].r2.low == 33);
        CHECK(ticket_info.field_descriptors[1].r2.high == 44);
        CHECK(ticket_info.field_descriptors[2].name == "seat");
        CHECK(ticket_info.field_descriptors[2].r1.low == 13);
        CHECK(ticket_info.field_descriptors[2].r1.high == 40);
        CHECK(ticket_info.field_descriptors[2].r2.low == 45);
        CHECK(ticket_info.field_descriptors[2].r2.high == 50);

        CHECK(ticket_info.your_ticket.fields == std::vector<int>{ 7, 1, 14 });

        REQUIRE(ticket_info.nearby_tickets.size() == 4);
        CHECK(ticket_info.nearby_tickets[0].fields == std::vector<int>{ 7, 3, 47 });
        CHECK(ticket_info.nearby_tickets[1].fields == std::vector<int>{ 40, 4, 50 });
        CHECK(ticket_info.nearby_tickets[2].fields == std::vector<int>{ 55, 2, 20 });
        CHECK(ticket_info.nearby_tickets[3].fields == std::vector<int>{ 38, 6, 12 });
    }

    SECTION("Invalid Fields")
    {
        auto const invalid_fields = invalidFields(ticket_info);
        CHECK(invalid_fields == std::vector<int>{ 4, 55, 12 });
    }

    SECTION("Ticket Scanning Error Rate")
    {
        CHECK(ticketScanningErrorRate(ticket_info) == 71);
    }

    SECTION("Discard Invalid Tickets")
    {
        auto filtered_tickets = ticket_info;
        discardInvalidTickets(filtered_tickets);
        REQUIRE(filtered_tickets.nearby_tickets.size() == 1);
        CHECK(filtered_tickets.nearby_tickets[0].fields == std::vector<int>{ 7, 3, 47 });
    }

    char const sample_input2[] = R"(class: 0-1 or 4-19
row: 0-5 or 8-19
seat: 0-13 or 16-19

your ticket:
11,12,13

nearby tickets:
3,9,18
15,1,5
5,14,9
)";

    SECTION("Map Ticket Fields")
    {
        auto ticket_info2 = parseInput(sample_input2);
        discardInvalidTickets(ticket_info2);
        CHECK(ticket_info2.nearby_tickets.size() == 3);

        auto field_index_map = mapTicketFields(ticket_info2);
        REQUIRE(field_index_map.size() == 3);
        CHECK(field_index_map["class"] == 1);
        CHECK(field_index_map["row"] == 0);
        CHECK(field_index_map["seat"] == 2);
    }
}
