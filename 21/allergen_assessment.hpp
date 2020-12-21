#ifndef ADVENT_OF_CODE_21_ALLERGEN_ASSESSMENT_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_21_ALLERGEN_ASSESSMENT_HPP_INCLUDE_GUARD

#include <fmt/format.h>

#include <array>
#include <bitset>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

struct FoodsList {
    std::vector<std::string> ingredients;
    std::vector<std::string> allergens;
    struct Food {
        std::vector<std::size_t> ingredient_index;
        std::vector<std::size_t> allergen_index;
    };
    std::vector<Food> foods;
};

FoodsList parseInput(std::string_view input);

struct AllergenIngredientMap {
    std::unordered_map<std::size_t, std::size_t> ingredients_by_allergen;
    std::unordered_map<std::size_t, std::size_t> allergens_by_ingredient;
};

AllergenIngredientMap solve(FoodsList const& fl);

int countNonAllergenOccurrences(FoodsList const& fl, AllergenIngredientMap const& aim);

std::string buildCanonicalDangerousIngredientsList(FoodsList const& fl, AllergenIngredientMap const& aim);

#endif
