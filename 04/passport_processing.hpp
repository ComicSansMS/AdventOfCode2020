#ifndef ADVENT_OF_CODE_04_PASSPORT_PROCESSING_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_04_PASSPORT_PROCESSING_HPP_INCLUDE_GUARD

#include <optional>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

struct Passport {
    std::optional<std::string> birthYear;
    std::optional<std::string> issueYear;
    std::optional<std::string> expirationYear;
    std::optional<std::string> height;
    std::optional<std::string> hairColor;
    std::optional<std::string> eyeColor;
    std::optional<std::string> passportId;
    std::optional<std::string> countryId;
};

std::vector<Passport> parseInput(std::string_view input);

bool isValid(Passport const& p);

std::size_t countValidPassports(std::vector<Passport> const& ps);

bool hasValidBirthYear(Passport const& p);

bool hasValidIssueYear(Passport const& p);

bool hasValidExpirationYear(Passport const& p);

bool hasValidHeight(Passport const& p);

bool hasValidHairColor(Passport const& p);

bool hasValidEyeColor(Passport const& p);

bool hasValidPassportId(Passport const& p);

bool isValid2(Passport const& p);

std::size_t countValidPassports2(std::vector<Passport> const& ps);

#endif
