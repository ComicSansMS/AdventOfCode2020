#ifndef ADVENT_OF_CODE_15_RAMBUNCTIOUS_RECITATION_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_15_RAMBUNCTIOUS_RECITATION_HPP_INCLUDE_GUARD

#include <array>
#include <bitset>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

std::vector<int64_t> parseInput(std::string_view input);

struct Mention {
    std::size_t last;
    std::size_t before_last;
    Mention();
    explicit Mention(std::size_t i);
};

void addMention(Mention& m, std::size_t turn);

bool isFirstMention(Mention const& m);

int64_t getAnswer(Mention const& m);

struct Game {
    std::vector<int64_t> turns;
    std::unordered_map<int64_t, Mention> last_mention;
};

Game playGame(std::vector<int64_t> input, std::size_t n_turns);

int64_t playGameFast(std::vector<int64_t> input, std::size_t n_turns);

#endif
