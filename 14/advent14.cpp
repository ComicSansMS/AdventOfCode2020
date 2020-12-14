#include <docking_data.hpp>

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

    auto const instructions = parseInput(*input);
    auto const result = runProgram(instructions);
    auto const result2 = runProgram2(instructions);

    fmt::print("First result is {}\n", result.sum_of_values);
    fmt::print("Second result is {}\n", result2.sum_of_values);

    return 0;
}
