#include <rambunctious_recitation.hpp>

#include <fmt/printf.h>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>

#include <algorithm>
#include <bitset>
#include <cassert>
#include <cctype>
#include <charconv>
#include <limits>
#include <numeric>
#include <regex>
#include <sstream>
#include <string>


std::vector<int64_t> parseInput(std::string_view input)
{
    std::string_view sv;
    return ranges::to<std::vector<int64_t>>(
        input |
        ranges::views::split(',') |
        ranges::views::transform([](auto sv) -> int64_t {
            int64_t i;
            std::from_chars(&(*sv.begin()), &(*sv.begin()) + ranges::distance(sv.begin(), sv.end()), i);
            return i;
        }));
}

Mention::Mention()
    :last(std::numeric_limits<std::size_t>::max()), before_last(std::numeric_limits<std::size_t>::max())
{}

Mention::Mention(std::size_t i)
    :last(i), before_last(std::numeric_limits<std::size_t>::max())
{}

void addMention(Mention& m, std::size_t turn)
{
    m.before_last = m.last;
    m.last = turn;
}

bool isFirstMention(Mention const& m)
{
    return m.before_last == std::numeric_limits<std::size_t>::max();
}

int64_t getAnswer(Mention const& m)
{
    return m.last - m.before_last;
}

Game playGame(std::vector<int64_t> input, std::size_t n_turns)
{
    Game ret;
    for(std::size_t i = 0; i < n_turns; ++i) {
        if(i < input.size()) {
            ret.turns.push_back(input[i]);
            ret.last_mention.insert(std::make_pair(input[i], Mention(i)));
        } else {
            int64_t const last_spoken_number = ret.turns.back();
            auto& mention = ret.last_mention[last_spoken_number];
            if (isFirstMention(mention)) {
                // number was not heard before
                ret.turns.push_back(0);
                addMention(ret.last_mention[0], i);
            } else {
                auto const answer = getAnswer(mention);
                ret.turns.push_back(answer);
                addMention(ret.last_mention[answer], i);
            }
        }
    }
    return ret;
}

int64_t playGameFast(std::vector<int64_t> input, std::size_t n_turns)
{
    int64_t last_spoken_number = -1;
    std::vector<Mention> last_mention(n_turns);
    for(std::size_t i = 0; i < n_turns; ++i) {
        if(i < input.size()) {
            last_spoken_number = input[i];
            last_mention[input[i]] = Mention(i);
        } else {
            auto& mention = last_mention[last_spoken_number];
            if (isFirstMention(mention)) {
                // number was not heard before
                last_spoken_number = 0;
                addMention(last_mention[0], i);
            } else {
                auto const answer = getAnswer(mention);
                last_spoken_number = answer;
                addMention(last_mention[answer], i);
            }
        }
    }
    return last_spoken_number;
}
