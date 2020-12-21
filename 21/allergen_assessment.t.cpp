#include <allergen_assessment.hpp>

#include <fmt/format.h>

#include <catch.hpp>

TEST_CASE("Allergen Assessment")
{
    char const sample_input[] = "mxmxvkd kfcds sqjhc nhms (contains dairy, fish)\n"
                                "trh fvjkl sbzzf mxmxvkd (contains dairy)\n"
                                "sqjhc fvjkl (contains soy)\n"
                                "sqjhc mxmxvkd sbzzf (contains fish)\n";

    SECTION("Parse Input")
    {
        auto const fl = parseInput(sample_input);
        REQUIRE(fl.ingredients.size() == 7);
        CHECK(fl.ingredients[0] == "mxmxvkd");
        CHECK(fl.ingredients[1] == "kfcds");
        CHECK(fl.ingredients[2] == "sqjhc");
        CHECK(fl.ingredients[3] == "nhms");
        CHECK(fl.ingredients[4] == "trh");
        CHECK(fl.ingredients[5] == "fvjkl");
        CHECK(fl.ingredients[6] == "sbzzf");

        REQUIRE(fl.allergens.size() == 3);
        CHECK(fl.allergens[0] == "dairy");
        CHECK(fl.allergens[1] == "fish");
        CHECK(fl.allergens[2] == "soy");

        REQUIRE(fl.foods.size() == 4);
        REQUIRE(fl.foods[0].ingredient_index == std::vector<std::size_t>{ 0, 1, 2, 3 });
        REQUIRE(fl.foods[0].allergen_index == std::vector<std::size_t>{ 0, 1 });

        REQUIRE(fl.foods[1].ingredient_index == std::vector<std::size_t>{ 4, 5, 6, 0 });
        REQUIRE(fl.foods[1].allergen_index == std::vector<std::size_t>{ 0 });

        REQUIRE(fl.foods[2].ingredient_index == std::vector<std::size_t>{ 2, 5 });
        REQUIRE(fl.foods[2].allergen_index == std::vector<std::size_t>{ 2 });

        REQUIRE(fl.foods[3].ingredient_index == std::vector<std::size_t>{ 2, 0, 6 });
        REQUIRE(fl.foods[3].allergen_index == std::vector<std::size_t>{ 1 });
    }

    SECTION("Solve")
    {
        auto const fl = parseInput(sample_input);
        auto const aim = solve(fl);
        CHECK(aim.allergens_by_ingredient.size() == 3);
        CHECK(aim.allergens_by_ingredient.find(0)->second == 0);
        CHECK(aim.allergens_by_ingredient.find(2)->second == 1);
        CHECK(aim.allergens_by_ingredient.find(5)->second == 2);
        CHECK(aim.ingredients_by_allergen.size() == 3);
        CHECK(aim.ingredients_by_allergen.find(0)->second == 0);
        CHECK(aim.ingredients_by_allergen.find(1)->second == 2);
        CHECK(aim.ingredients_by_allergen.find(2)->second == 5);
    }

    SECTION("Count Non Allergen Occurrences")
    {
        auto const fl = parseInput(sample_input);
        auto const aim = solve(fl);
        CHECK(countNonAllergenOccurrences(fl, aim) == 5);
    }

    SECTION("Build Canonical Dangerous Ingredients List")
    {
        auto const fl = parseInput(sample_input);
        auto const aim = solve(fl);
        CHECK(buildCanonicalDangerousIngredientsList(fl, aim) == "mxmxvkd,sqjhc,fvjkl");
    }
}
