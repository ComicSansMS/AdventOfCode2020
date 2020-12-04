#include <passport_processing.hpp>

#include <algorithm>
#include <cassert>
#include <cctype>
#include <limits>
#include <regex>
#include <sstream>
#include <string>

std::vector<Passport> parseInput(std::string_view input)
{
    std::vector<Passport> ret;
    ret.emplace_back();
    std::stringstream sstr{ std::string{input} };
    std::string line;
    std::regex rx_entry(R"((\w\w\w):([^\s]+))");
    while (std::getline(sstr, line)) {
        if (line.empty()) { ret.emplace_back(); }
        for (auto it = std::sregex_iterator(line.begin(), line.end(), rx_entry), it_end = std::sregex_iterator();
             it != it_end;
             ++it)
        {
            std::smatch m = *it;
            auto const set_entry = [&ret](std::optional<std::string> Passport::* field, std::string const& str) {
                assert(!(ret.back().*field));
                ret.back().*field = str;
            };
            if (m[1] == "byr") {
                set_entry(&Passport::birthYear, m[2]);
            } else if(m[1] == "iyr") {
                set_entry(&Passport::issueYear, m[2]);
            } else if(m[1] == "eyr") {
                set_entry(&Passport::expirationYear, m[2]);
            } else if(m[1] == "hgt") {
                set_entry(&Passport::height, m[2]);
            } else if(m[1] == "hcl") {
                set_entry(&Passport::hairColor, m[2]);
            } else if(m[1] == "ecl") {
                set_entry(&Passport::eyeColor, m[2]);
            } else if(m[1] == "pid") {
                set_entry(&Passport::passportId, m[2]);
            } else if (m[1] == "cid") {
                set_entry(&Passport::countryId, m[2]);
            } else {
                assert(false);
            }
        }
    }
    return ret;
}

bool isValid(Passport const& p)
{
    return (p.birthYear && p.issueYear && p.expirationYear && p.height && p.hairColor && p.eyeColor && p.passportId);
}

std::size_t countValidPassports(std::vector<Passport> const& ps)
{
    return std::count_if(begin(ps), end(ps), isValid);
}

bool hasValidBirthYear(Passport const& p)
{
    if ((p.birthYear) && (p.birthYear->size() == 4) &&
        (std::all_of(p.birthYear->begin(), p.birthYear->end(), [](char c) -> bool { return std::isdigit(c) != 0; }))) {
        int const y = std::stoi(*p.birthYear);
        return (y >= 1920) && (y <= 2002);
    } else {
        return false;
    }
}

bool hasValidIssueYear(Passport const& p)
{
    if ((p.issueYear) && (p.issueYear->size() == 4) &&
        (std::all_of(p.issueYear->begin(), p.issueYear->end(), [](char c) -> bool { return std::isdigit(c) != 0; }))) {
        int const y = std::stoi(*p.issueYear);
        return (y >= 2010) && (y <= 2020);
    } else {
        return false;
    }
}


bool hasValidExpirationYear(Passport const& p)
{
    if ((p.expirationYear) && (p.expirationYear->size() == 4) &&
        (std::all_of(p.expirationYear->begin(), p.expirationYear->end(), [](char c) -> bool { return std::isdigit(c) != 0; }))) {
        int const y = std::stoi(*p.expirationYear);
        return (y >= 2020) && (y <= 2030);
    } else {
        return false;
    }
}

bool hasValidHeight(Passport const& p)
{
    if ((p.height) && (p.height->ends_with("in") || p.height->ends_with("cm")) &&
        (std::all_of(p.height->begin(), p.height->end() - 2, [](char c) -> bool { return std::isdigit(c) != 0; }))) {
        int const h = std::stoi(p.height->substr(0, p.height->size() - 2));
        if (p.height->ends_with("cm")) {
            return (h >= 150) && (h <= 193);
        } else {
            return (h >= 59) && (h <= 76);
        }
    } else {
        return false;
    }
}

bool hasValidHairColor(Passport const& p)
{
    if ((p.hairColor) && (p.hairColor->size() == 7) && ((*p.hairColor)[0] == '#')) {
        return std::all_of(p.hairColor->begin() + 1, p.hairColor->end(),
                           [](char c) -> bool { return (std::isdigit(c) != 0) || ((c >= 'a') && (c <= 'f')); });
    } else {
        return false;
    }
}

bool hasValidEyeColor(Passport const& p)
{
    if ((p.eyeColor) && (p.eyeColor->size() == 3)) {
        return ((*p.eyeColor == "amb") ||
                (*p.eyeColor == "blu") ||
                (*p.eyeColor == "brn") ||
                (*p.eyeColor == "gry") ||
                (*p.eyeColor == "grn") ||
                (*p.eyeColor == "hzl") ||
                (*p.eyeColor == "oth"));
    } else {
        return false;
    }
}

bool hasValidPassportId(Passport const& p)
{
    if ((p.passportId) && (p.passportId->size() == 9) &&
        (std::all_of(p.passportId->begin(), p.passportId->end(), [](char c) -> bool { return std::isdigit(c) != 0; }))) {
        return true;
    } else {
        return false;
    }
}

bool isValid2(Passport const& p)
{
    return hasValidBirthYear(p) && hasValidIssueYear(p) && hasValidExpirationYear(p) && hasValidHeight(p) &&
           hasValidHairColor(p) && hasValidEyeColor(p) && hasValidPassportId(p);
}

std::size_t countValidPassports2(std::vector<Passport> const& ps)
{
    return std::count_if(begin(ps), end(ps), isValid2);
}
