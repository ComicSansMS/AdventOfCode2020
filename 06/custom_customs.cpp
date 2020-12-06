#include <custom_customs.hpp>


#include <algorithm>
#include <bitset>
#include <cassert>
#include <cctype>
#include <limits>
#include <numeric>
#include <regex>
#include <sstream>
#include <string>

std::vector<Group> parseInput(std::string_view input)
{
    std::vector<Group> ret;
    bool new_group = true;
    bool new_passenger = true;
    for (auto const& c : input) {
        if (c == '\n') {
            if (!new_passenger) {
                new_passenger = true;
            } else {
                assert(!new_group);
                new_group = true;
            }
        } else {
            assert((c >= 'a') && (c <= 'z'));
            if (new_group) {
                ret.emplace_back();
                new_group = false;
            }
            if (new_passenger) {
                ret.back().answers.emplace_back();
                new_passenger = false;
            }
            ret.back().answers.back()[c - 'a'] = true;
            ret.back().group_answers[c - 'a'] = true;
        }
    }
    return ret;
}

int countYesAnswers(Answers const& a)
{
    return static_cast<int>(std::count(a.begin(), a.end(), true));
}

int sumOfGroupYesAnswers(std::vector<Group> const& g)
{
    return std::accumulate(g.begin(), g.end(), 0,
                           [](int acc, Group const& g) { return acc + countYesAnswers(g.group_answers); });
}

int countAllYesAnswersInGroup(Group const& g)
{
    std::bitset<26> yes_bits(0xffffffff);
    for (auto const& a : g.answers) {
        for (int i = 0; i < a.size(); ++i) {
            if (!a[i]) {
                yes_bits.reset(i);
            }
        }
    }
    return static_cast<int>(yes_bits.count());
}

int countAllYesAnswers(std::vector<Group> const& g)
{
    return std::accumulate(g.begin(), g.end(), 0,
                           [](int acc, Group const& g) { return acc + countAllYesAnswersInGroup(g); });
}
