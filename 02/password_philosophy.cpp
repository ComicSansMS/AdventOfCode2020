#include <password_philosophy.hpp>

#include <algorithm>
#include <cassert>
#include <limits>
#include <regex>
#include <sstream>
#include <string>

std::vector<PasswordPhilosophy> parseInput(std::string_view input)
{
    std::regex rx_line(R"((\d+)-(\d+) (\w): (\w+)[\r\n]?)");

    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(begin(input), end(input), rx_line);
    auto const it_end = regex_it();

    std::vector<PasswordPhilosophy> ret;
    std::transform(it_begin, it_end, std::back_inserter(ret),
                   [](std::match_results<std::string_view::iterator> const& match) -> PasswordPhilosophy {
                       PasswordPhilosophy ret;
                       ret.min = std::stoi(match[1]);
                       ret.max = std::stoi(match[2]);
                       ret.c = *match[3].first;
                       ret.password = match[4];
                       return ret;
                   });
    return ret;
}

bool checkPassword(PasswordPhilosophy const& c)
{
    auto const ccount = std::count(c.password.begin(), c.password.end(), c.c);
    return (ccount >= c.min) && (ccount <= c.max);
}

int countValidPasswords(std::vector<PasswordPhilosophy> const& p)
{
    assert(p.size() < std::numeric_limits<int>::max());
    return static_cast<int>(std::count_if(p.begin(), p.end(), checkPassword));
}

bool checkPassword2(PasswordPhilosophy const& c)
{
    char const c1 = c.password[c.min - 1];
    char const c2 = c.password[c.max - 1];
    return ((c1 == c.c) ? 1 : 0) + ((c2 == c.c) ? 1 : 0) == 1;
}

int countValidPasswords2(std::vector<PasswordPhilosophy> const& p)
{
    assert(p.size() < std::numeric_limits<int>::max());
    return static_cast<int>(std::count_if(p.begin(), p.end(), checkPassword2));
}


