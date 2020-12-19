#include <operation_order.hpp>

#include <fmt/printf.h>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>

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

    auto const nodes = parseInput(*input);
    auto const nodes2 = ranges::to<std::vector<Node>>(nodes | ranges::views::transform(changeAdditionPrecedence));

    fmt::print("First result is {}\n", evaluateAndSum(nodes));
    fmt::print("Second result is {}\n", evaluateAndSum(nodes2));

    return 0;
}
