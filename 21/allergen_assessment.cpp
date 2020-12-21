#include <allergen_assessment.hpp>

#include <fmt/printf.h>

#include <range/v3/view/cartesian_product.hpp>
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

FoodsList parseInput(std::string_view input)
{
    FoodsList l;
    auto getIngredientIndex = [&l](std::string&& s) -> std::size_t {
        auto it = std::find(begin(l.ingredients), end(l.ingredients), s);
        if (it == end(l.ingredients)) {
            l.ingredients.emplace_back(std::move(s));
            return l.ingredients.size() - 1;
        }
        return std::distance(begin(l.ingredients), it);
    };
    auto getAllergenIndex = [&l](std::string&& s) -> std::size_t {
        auto it = std::find(begin(l.allergens), end(l.allergens), s);
        if (it == end(l.allergens)) {
            l.allergens.emplace_back(std::move(s));
            return l.allergens.size() - 1;
        }
        return std::distance(begin(l.allergens), it);
    };
    std::string_view::iterator it = input.begin();
    std::string_view::iterator const it_end = input.end();
    for (;;) {
        auto const it_eol = std::find(it, it_end, '\n');
        if (it_eol == it_end) { break; }
        l.foods.emplace_back();
        auto const it_ingredients_end = std::find(it, it_eol, '(');
        for (;;) {
            auto const it_next = std::find(it, it_ingredients_end, ' ');
            if (it_next == it_ingredients_end) { break; }
            std::size_t const ingredient_index = getIngredientIndex(std::string(it, it_next));
            l.foods.back().ingredient_index.push_back(ingredient_index);
            it = it_next + 1;
        }
        it = it_ingredients_end + 10;
        std::string acc;
        for (; it < it_eol; ++it) {
            char c = *it;
            if ((c == ',') || (c == ')')) {
                assert(!acc.empty());
                std::size_t const allergen_index = getAllergenIndex(std::move(acc));
                acc.clear();
                l.foods.back().allergen_index.push_back(allergen_index);
            } else if(c == ' ') {
                // ignore whitespace
                assert(acc.empty());
            } else {
                acc.push_back(c);
            }
        }
        assert(it == it_eol);
        ++it;
    }
    return l;
}

std::vector<std::size_t> commonIngredients(FoodsList const& fl, std::vector<size_t> foods_to_check)
{
    assert(!foods_to_check.empty());
    std::vector<std::size_t> ret;
    for (auto const& f : fl.foods[foods_to_check.front()].ingredient_index) {
        if (std::all_of(begin(foods_to_check), end(foods_to_check), [&fl, f](std::size_t food_index) {
                auto const& ingr = fl.foods[food_index].ingredient_index;
                return std::find(begin(ingr), end(ingr), f) != end(ingr);
            })) {
            ret.push_back(f);
        }
    }
    return ret;
}

AllergenIngredientMap solve(FoodsList const& fl)
{
    AllergenIngredientMap aim;
    std::vector<std::vector<std::size_t>> foods_with_allergen;
    foods_with_allergen.reserve(fl.allergens.size());
    for (std::size_t i_allergen = 0; i_allergen < fl.allergens.size(); ++i_allergen) {
        foods_with_allergen.emplace_back();
        for (std::size_t i_foods = 0; i_foods < fl.foods.size(); ++i_foods) {
            auto const& food = fl.foods[i_foods];
            if (std::find(begin(food.allergen_index), end(food.allergen_index), i_allergen) != end(food.allergen_index)) {
                foods_with_allergen.back().push_back(i_foods);
            }
        }
    }
    while (aim.ingredients_by_allergen.size() < fl.allergens.size()) {
        for (std::size_t i_allergen = 0; i_allergen < fl.allergens.size(); ++i_allergen) {
            if (aim.ingredients_by_allergen.contains(i_allergen)) { continue; }
            auto common_ingr = commonIngredients(fl, foods_with_allergen[i_allergen]);
            // filter out ingredients already assigned
            common_ingr.erase(
                std::remove_if(begin(common_ingr), end(common_ingr),
                               [&aim, i_allergen](std::size_t ingr) -> bool
                               {
                                   auto it = aim.allergens_by_ingredient.find(ingr);
                                   if (it != end(aim.allergens_by_ingredient)) {
                                       assert(it->second != i_allergen);
                                       return true;
                                   }
                                   return false;
                               }),
                end(common_ingr));
            // ***
            if (common_ingr.size() == 1) {
                aim.ingredients_by_allergen[i_allergen] = common_ingr.front();
                aim.allergens_by_ingredient[common_ingr.front()] = i_allergen;
            }
        }
    }
    return aim;
}

int countNonAllergenOccurrences(FoodsList const& fl, AllergenIngredientMap const& aim)
{
    int acc = 0;
    for (auto const& food : fl.foods) {
        for (auto const& ingr : food.ingredient_index) {
            if (!aim.allergens_by_ingredient.contains(ingr)) { ++acc; }
        }
    }
    return acc;
}

struct AllergenIndexPair {
    std::string name;
    std::size_t index;

    friend bool operator<(AllergenIndexPair const& lhs, AllergenIndexPair const& rhs) {
        return lhs.name < rhs.name;
    }
};

std::string buildCanonicalDangerousIngredientsList(FoodsList const& fl, AllergenIngredientMap const& aim)
{
    std::vector<AllergenIndexPair> allergens;
    for (std::size_t i = 0; i < fl.allergens.size(); ++i) {
        allergens.push_back(AllergenIndexPair{ fl.allergens[i], i });
    }
    std::sort(begin(allergens), end(allergens));
    std::string acc = "";
    for (auto const& [allergen_name, allergen_index] : allergens) {
        std::size_t const ingredients_index = aim.ingredients_by_allergen.find(allergen_index)->second;
        if (!acc.empty()) { acc += ","; }
        acc += fl.ingredients[ingredients_index];
    }
    return acc;
}
