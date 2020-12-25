#include <combo_breaker.hpp>

#include <fmt/printf.h>

#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/range/conversion.hpp>

#include <algorithm>
#include <bitset>
#include <cassert>
#include <cctype>
#include <charconv>
#include <forward_list>
#include <iterator>
#include <limits>
#include <numeric>
#include <regex>
#include <sstream>
#include <string>
#include <memory_resource>

PublicKeys parseInput(std::string_view input)
{
    PublicKeys ret;
    auto const it_eol1 = std::find(input.begin(), input.end(), '\n');
    std::from_chars(&(*input.begin()), &(*it_eol1), ret.card_key);
    auto const it_eol2 = std::find(it_eol1 + 1, input.end(), '\n');
    std::from_chars(&(*(it_eol1 + 1)), &(*it_eol2), ret.door_key);
    return ret;
}

int64_t crackSecretLoopSize(PublicKey k)
{
    int64_t const subject_number = 7;
    int64_t val = 1;
    int64_t ret;
    for (ret = 0; val != k; ++ret) {
        val = (val * subject_number) % 20201227;
    }
    return ret;
}

int64_t crackEncryptionKey(PublicKeys const& pk)
{
    auto const [smaller_key, bigger_key] = std::minmax(pk.card_key, pk.door_key);
    int64_t const secret_loop_size = crackSecretLoopSize(smaller_key);
    int64_t val = 1;
    for (int i = 0; i < secret_loop_size; ++i) {
        val = (val * bigger_key) % 20201227;
    }
    return val;
}
