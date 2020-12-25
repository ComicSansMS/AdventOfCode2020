#ifndef ADVENT_OF_CODE_25_COMBO_BREAKER_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_25_COMBO_BREAKER_HPP_INCLUDE_GUARD

#include <fmt/format.h>

#include <array>
#include <bitset>
#include <deque>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

using PublicKey = int64_t;

struct PublicKeys {
    PublicKey door_key;
    PublicKey card_key;
};

PublicKeys parseInput(std::string_view input);

int64_t crackSecretLoopSize(PublicKey k);

int64_t crackEncryptionKey(PublicKeys const& pk);

#endif
