#include <combo_breaker.hpp>

#include <fmt/format.h>

#include <catch.hpp>

TEST_CASE("Combo Breaker")
{
    SECTION("Parse Input")
    {
        auto const keys = parseInput("12345\n67890\n");
        CHECK(keys.card_key == 12345);
        CHECK(keys.door_key == 67890);
    }

    PublicKey const sample_card_key = 5764801;
    PublicKey const sample_door_key = 17807724;

    SECTION("Crack Public Key")
    {
        CHECK(crackSecretLoopSize(sample_card_key) == 8);
        CHECK(crackSecretLoopSize(sample_door_key) == 11);
    }

    SECTION("Crack Encryption Key")
    {
        PublicKeys pk{sample_card_key, sample_door_key};
        CHECK(crackEncryptionKey(pk) == 14897079);
    }
}
