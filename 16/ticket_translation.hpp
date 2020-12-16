#ifndef ADVENT_OF_CODE_16_TICKET_TRANSLATION_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_16_TICKET_TRANSLATION_HPP_INCLUDE_GUARD

#include <array>
#include <bitset>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

struct Range {
    int low;
    int high;
};

struct FieldDescriptor {
    std::string name;
    Range r1;
    Range r2;
};

struct Ticket {
    std::vector<int> fields;
};

struct TicketInfo {
    std::vector<FieldDescriptor> field_descriptors;
    Ticket your_ticket;
    std::vector<Ticket> nearby_tickets;
};

TicketInfo parseInput(std::string_view input);

bool checkRange(int i, Range const& r);

bool checkField(int i, FieldDescriptor const& fd);

std::vector<int> invalidFields(TicketInfo const& ti);

int64_t ticketScanningErrorRate(TicketInfo const& ti);

void discardInvalidTickets(TicketInfo& ti);

std::unordered_map<std::string, std::size_t> mapTicketFields(TicketInfo ti);

int64_t solveTicket(TicketInfo ti);

#endif
