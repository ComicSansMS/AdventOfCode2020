#include <crab_combat.hpp>

#include <fmt/printf.h>

#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/range/conversion.hpp>

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

Game parseInput(std::string_view input)
{
    Game ret;
    Deck* current_deck = nullptr;
    for (auto line : input | ranges::views::split('\n') | 
                     ranges::views::transform([](auto&& rsv) { return std::string_view(&(*rsv.begin()), ranges::distance(rsv)); }) )
    {
        if (line == "Player 1:") {
            assert(current_deck == nullptr);
            current_deck = &ret.player1;
        } else if(line == "Player 2:") {
            assert(current_deck == &ret.player1);
            current_deck = &ret.player2;
        } else if(!line.empty()) {
            int64_t tmp;
            auto const res = std::from_chars(&line.front(), &line.back() + 1, tmp);
            assert(res.ec == std::errc{});
            current_deck->push_back(tmp);
        } else {
            // ignore empty line
        }
    }
    return ret;
}

Game playRound(Game g)
{
    auto const c1 = g.player1.front();
    g.player1.pop_front();
    auto const c2 = g.player2.front();
    g.player2.pop_front();

    if (c1 > c2) {
        g.player1.push_back(c1);
        g.player1.push_back(c2);
    } else {
        assert(c2 > c1);
        g.player2.push_back(c2);
        g.player2.push_back(c1);
    }
    return g;
}

Result playGame(Game g)
{
    while ((!g.player1.empty()) && (!g.player2.empty())) {
        g = playRound(g);
    }
    Deck const& winning_deck = (g.player1.empty()) ? g.player2 : g.player1;
    Result res;
    res.score = 0;
    for (std::size_t i = 0; i < winning_deck.size(); ++i) {
        res.score += winning_deck[winning_deck.size() - i - 1] * static_cast<int64_t>(i + 1);
    }
    res.g = std::move(g);
    return res;
}

Result playGameRecursive(Game const& base_game)
{
    std::vector<std::vector<Game>> game_stack;
    game_stack.emplace_back();
    game_stack.back().push_back(base_game);
    auto win_player1 = [&game_stack]() {
        game_stack.pop_back();
        auto& current_game = game_stack.back();
        Game g = current_game.back();
        auto const& c1 = g.player1.front();
        g.player1.pop_front();
        auto const& c2 = g.player2.front();
        g.player2.pop_front();
        g.player1.push_back(c1);
        g.player1.push_back(c2);
        current_game.push_back(std::move(g));
    };
    auto win_player2 = [&game_stack]() {
        game_stack.pop_back();
        auto& current_game = game_stack.back();
        Game g = current_game.back();
        auto const& c1 = g.player1.front();
        g.player1.pop_front();
        auto const& c2 = g.player2.front();
        g.player2.pop_front();
        g.player2.push_back(c2);
        g.player2.push_back(c1);
        current_game.push_back(std::move(g));
    };
    for (;;) {
        auto& current_game = game_stack.back();
        auto g = current_game.back();
        if (std::find(current_game.begin(), current_game.end(), g) != current_game.end() - 1) {
            // player 1 insta-win
            assert(game_stack.size() > 1);
            win_player1();
            continue;
        }
        auto const& c1 = g.player1.front();
        g.player1.pop_front();
        auto const& c2 = g.player2.front();
        g.player2.pop_front();
        if ((c1 <= static_cast<int64_t>(g.player1.size())) && (c2 <= static_cast<int64_t>(g.player2.size()))) {
            // recurse
            Game rec;
            std::copy(g.player1.begin(), g.player1.begin() + c1, std::back_inserter(rec.player1));
            std::copy(g.player2.begin(), g.player2.begin() + c2, std::back_inserter(rec.player2));
            game_stack.emplace_back();
            game_stack.back().emplace_back(std::move(rec));
            continue;
        } else {
            if (c1 > c2) {
                g.player1.push_back(c1);
                g.player1.push_back(c2);
            } else {
                assert(c2 > c1);
                g.player2.push_back(c2);
                g.player2.push_back(c1);
            }
            if (g.player1.empty()) {
                // player 2 won
                if (game_stack.size() == 1) { return playGame(g); }
                win_player2();
                continue;
            } else if(g.player2.empty()) {
                // player 1 won
                if (game_stack.size() == 1) { return playGame(g); }
                win_player1();
                continue;
            } else {
                current_game.emplace_back(std::move(g));
            }
            continue;
        }
    }
}
