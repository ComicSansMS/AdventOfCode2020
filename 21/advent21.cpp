#include <allergen_assessment.hpp>

#include <fmt/printf.h>

#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

std::optional<std::string> readInput(char const* filename)
{
    std::ifstream fin(filename);
    if(!fin) {
        fmt::print(std::cerr, "Unable to open input file '{}' for reading.\n", filename);
        return std::nullopt;
    }

    std::stringstream sstr;
    sstr << fin.rdbuf();
    if(!fin) {
        fmt::print(std::cerr, "Unable to read input from file '{}'.\n", filename);
        return std::nullopt;
    }
    return sstr.str();
}

int main(int argc, char* argv[])
{
    char const* input_filename = "input";
    if(argc == 2) {
        input_filename = argv[1];
    }

    auto const input = readInput(input_filename);

    if(!input) {
        return 1;
    }

    auto const foods_list = parseInput(*input);
    auto const aim = solve(foods_list);

    fmt::print("First result is {}\n", countNonAllergenOccurrences(foods_list, aim));
    fmt::print("Second result is {}\n", buildCanonicalDangerousIngredientsList(foods_list, aim));

    return 0;
}
