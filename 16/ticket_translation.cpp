#include <ticket_translation.hpp>

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


TicketInfo parseInput(std::string_view input)
{
    std::regex rx_prelude(R"(([^:]+): (\d+)-(\d+) or (\d+)-(\d+))");
    std::regex rx_ticket_fields(R"((\d)+)");

    TicketInfo ret;
    std::stringstream sstr{std::string{input}};
    std::string line;
    enum class Parse {
        Prelude,
        YourTicket,
        NearbyTickets,
    } parse_state = Parse::Prelude;
    bool section_done = false;
    while (std::getline(sstr, line)) {
        if(!section_done) {
            if(line == "") {
                section_done = true;
            } else if(parse_state == Parse::Prelude) {
                std::smatch smatch;
                std::regex_match(line, smatch, rx_prelude);
                FieldDescriptor fd;
                fd.name = smatch[1];
                fd.r1.low = std::stoi(smatch[2]);
                fd.r1.high = std::stoi(smatch[3]);
                fd.r2.low = std::stoi(smatch[4]);
                fd.r2.high = std::stoi(smatch[5]);
                ret.field_descriptors.push_back(std::move(fd));
            } else if(parse_state == Parse::YourTicket) {
                std::sregex_iterator it_begin(begin(line), end(line), rx_ticket_fields);
                std::sregex_iterator it_end;
                ret.your_ticket.fields.reserve(ret.field_descriptors.size());
                std::transform(it_begin, it_end, std::back_inserter(ret.your_ticket.fields),
                               [](auto&& sm) -> int { return std::stoi(sm[0]); });
                assert(ret.your_ticket.fields.size() == ret.field_descriptors.size());
            } else {
                assert(parse_state == Parse::NearbyTickets);
                std::sregex_iterator it_begin(begin(line), end(line), rx_ticket_fields);
                std::sregex_iterator it_end;
                ret.nearby_tickets.emplace_back();
                ret.nearby_tickets.back().fields.reserve(ret.field_descriptors.size());
                std::transform(it_begin, it_end, std::back_inserter(ret.nearby_tickets.back().fields),
                               [](auto&& sm) -> int { return std::stoi(sm[0]); });
                assert(ret.nearby_tickets.back().fields.size() == ret.field_descriptors.size());
            }
        } else {
            if (parse_state == Parse::Prelude) {
                assert(line == "your ticket:");
                parse_state = Parse::YourTicket;
            } else {
                assert(parse_state == Parse::YourTicket);
                parse_state = Parse::NearbyTickets;
            }
            section_done = false;
        }
    }
    return ret;
}

bool checkRange(int i, Range const& r)
{
    return (i >= r.low) && (i <= r.high);
}

bool checkField(int i, FieldDescriptor const& fd)
{
    return checkRange(i, fd.r1) || checkRange(i, fd.r2);
}

std::vector<int> invalidFields(TicketInfo const& ti)
{
    std::vector<int> ret;
    for(auto const& t : ti.nearby_tickets) {
        for(auto const& f : t.fields) {
            if (std::none_of(begin(ti.field_descriptors), end(ti.field_descriptors),
                             [f](FieldDescriptor const& fd) { return checkField(f, fd); })) {
                ret.push_back(f);
            }
        }
    }
    return ret;
}

int64_t ticketScanningErrorRate(TicketInfo const& ti)
{
    auto const invalid_fields = invalidFields(ti);
    return std::accumulate(begin(invalid_fields), end(invalid_fields), int64_t{0});
}

void discardInvalidTickets(TicketInfo& ti)
{
    auto isInvalidTicket = [&ti](Ticket const& t) {
        return std::any_of(begin(t.fields), end(t.fields), [&ti](int field) {
            return std::none_of(begin(ti.field_descriptors), end(ti.field_descriptors),
                [field](FieldDescriptor const& fd) {
                    return checkField(field, fd);
                });
        });
    };
    ti.nearby_tickets.erase(
        std::remove_if(begin(ti.nearby_tickets), end(ti.nearby_tickets), isInvalidTicket),
        end(ti.nearby_tickets));
}

std::unordered_map<std::string, std::size_t> mapTicketFields(TicketInfo ti)
{
    ti.nearby_tickets.push_back(ti.your_ticket);
    std::vector<std::vector<std::size_t>> field_candidates;
    field_candidates.reserve(ti.field_descriptors.size());
    for (std::size_t field_index = 0; field_index < ti.field_descriptors.size(); ++field_index) {
        auto const& fd = ti.field_descriptors[field_index];
        field_candidates.emplace_back();
        for (std::size_t ticket_index = 0; ticket_index < ti.field_descriptors.size(); ++ticket_index) {
            bool does_match =
                std::all_of(begin(ti.nearby_tickets), end(ti.nearby_tickets), [&fd, ticket_index](Ticket const& ticket) {
                    return checkField(ticket.fields[ticket_index], fd);
                });
            if (does_match) { field_candidates.back().push_back(ticket_index); }
        }
    }

    std::unordered_map<std::string, std::size_t> field_index_map;
    std::vector<int> field_assigned(ti.field_descriptors.size(), 0);
    while(field_index_map.size() != ti.field_descriptors.size()) {
        for (std::size_t field_candidate_index = 0;
             field_candidate_index < field_candidates.size();
             ++field_candidate_index)
        {
            auto& fc = field_candidates[field_candidate_index];
            if((fc.size() == 1) && (field_assigned[field_candidate_index] != 1)) {
                field_index_map[ti.field_descriptors[field_candidate_index].name] = fc.back();
                for (std::size_t i = 0; i < field_candidates.size(); ++i) {
                    if (i != field_candidate_index) {
                        field_candidates[i].erase(
                            std::remove(field_candidates[i].begin(), field_candidates[i].end(), fc.back()),
                            field_candidates[i].end());
                    }
                }
                field_assigned[field_candidate_index] = 1;
            }
        }
    }
    return field_index_map;
}

int64_t solveTicket(TicketInfo ti)
{
    discardInvalidTickets(ti);
    auto const field_index_map = mapTicketFields(ti);
    std::vector<std::size_t> field_indices;
    for(auto const& [field_name, field_index] : field_index_map) {
        if(field_name.starts_with("departure")) {
            field_indices.push_back(field_index);
        }
    }
    assert(field_indices.size() == 6);
    int64_t const acc =
        std::accumulate(begin(field_indices), end(field_indices), int64_t{1},
            [&ti](int64_t acc, std::size_t index) -> int64_t {
                return acc * ti.your_ticket.fields[index];
            });
    return acc;
}
