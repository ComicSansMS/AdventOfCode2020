#ifndef ADVENT_OF_CODE_02_PASSWORD_PHILOSOPHY_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_02_PASSWORD_PHILOSOPHY_HPP_INCLUDE_GUARD

#include <string>
#include <string_view>
#include <vector>

struct PasswordPhilosophy {
    int min;
    int max;
    char c;
    std::string password;
};

std::vector<PasswordPhilosophy> parseInput(std::string_view input);

bool checkPassword(PasswordPhilosophy const& c);

int countValidPasswords(std::vector<PasswordPhilosophy> const& p);

bool checkPassword2(PasswordPhilosophy const& c);

int countValidPasswords2(std::vector<PasswordPhilosophy> const& p);

#endif
