#include <passport_processing.hpp>

#include <catch.hpp>

TEST_CASE("Passport Processing")
{
    char const sample_input[] =
R"(ecl:gry pid:860033327 eyr:2020 hcl:#fffffd
byr:1937 iyr:2017 cid:147 hgt:183cm

iyr:2013 ecl:amb cid:350 eyr:2023 pid:028048884
hcl:#cfa07d byr:1929

hcl:#ae17e1 iyr:2013
eyr:2024
ecl:brn pid:760753108 byr:1931
hgt:179cm

hcl:#cfa07d eyr:2025 pid:166559648
iyr:2011 ecl:brn hgt:59in
)";

    auto const passports = parseInput(sample_input);

    SECTION("Parse Input")
    {
        REQUIRE(passports.size() == 4);

        CHECK(*passports[0].birthYear == "1937");
        CHECK(*passports[0].issueYear == "2017");
        CHECK(*passports[0].expirationYear == "2020");
        CHECK(*passports[0].height == "183cm");
        CHECK(*passports[0].hairColor == "#fffffd");
        CHECK(*passports[0].eyeColor == "gry");
        CHECK(*passports[0].passportId == "860033327");
        CHECK(*passports[0].countryId == "147");

        CHECK(*passports[1].birthYear == "1929");
        CHECK(*passports[1].issueYear == "2013");
        CHECK(*passports[1].expirationYear == "2023");
        CHECK(!passports[1].height);
        CHECK(*passports[1].hairColor == "#cfa07d");
        CHECK(*passports[1].eyeColor == "amb");
        CHECK(*passports[1].passportId == "028048884");
        CHECK(*passports[1].countryId == "350");
    }

    SECTION("Passport Is Valid")
    {
        CHECK(isValid(passports[0]));
        CHECK(!isValid(passports[1]));
        CHECK(isValid(passports[2]));
        CHECK(!isValid(passports[3]));
    }

    SECTION("Count Valid Passports")
    {
        CHECK(countValidPassports(passports) == 2);
    }

    SECTION("Valid Birth Year")
    {
        Passport p;
        CHECK(!hasValidBirthYear(p));
        p.birthYear = "";
        CHECK(!hasValidBirthYear(p));
        p.birthYear = "111";
        CHECK(!hasValidBirthYear(p));
        p.birthYear = "10ab";
        CHECK(!hasValidBirthYear(p));
        p.birthYear = "1919";
        CHECK(!hasValidBirthYear(p));
        p.birthYear = "1920";
        CHECK(hasValidBirthYear(p));
        p.birthYear = "1995";
        CHECK(hasValidBirthYear(p));
        p.birthYear = "2002";
        CHECK(hasValidBirthYear(p));
        p.birthYear = "2003";
        CHECK(!hasValidBirthYear(p));
    }

    SECTION("Valid Issue Year")
    {
        Passport p;
        CHECK(!hasValidIssueYear(p));
        p.issueYear = "";
        CHECK(!hasValidIssueYear(p));
        p.issueYear = "111";
        CHECK(!hasValidIssueYear(p));
        p.issueYear = "10ab";
        CHECK(!hasValidIssueYear(p));
        p.issueYear = "2009";
        CHECK(!hasValidIssueYear(p));
        p.issueYear = "2010";
        CHECK(hasValidIssueYear(p));
        p.issueYear = "2015";
        CHECK(hasValidIssueYear(p));
        p.issueYear = "2020";
        CHECK(hasValidIssueYear(p));
        p.issueYear = "2021";
        CHECK(!hasValidIssueYear(p));
    }

    SECTION("Valid Expiration Year")
    {
        Passport p;
        CHECK(!hasValidExpirationYear(p));
        p.expirationYear = "";
        CHECK(!hasValidExpirationYear(p));
        p.expirationYear = "111";
        CHECK(!hasValidExpirationYear(p));
        p.expirationYear = "10ab";
        CHECK(!hasValidExpirationYear(p));
        p.expirationYear = "2019";
        CHECK(!hasValidExpirationYear(p));
        p.expirationYear = "2020";
        CHECK(hasValidExpirationYear(p));
        p.expirationYear = "2025";
        CHECK(hasValidExpirationYear(p));
        p.expirationYear = "2030";
        CHECK(hasValidExpirationYear(p));
        p.expirationYear = "2031";
        CHECK(!hasValidExpirationYear(p));
    }

    SECTION("Valid Height")
    {
        Passport p;
        CHECK(!hasValidHeight(p));
        p.height = "";
        CHECK(!hasValidHeight(p));
        p.height = "111";
        CHECK(!hasValidHeight(p));
        p.height = "10ab";
        CHECK(!hasValidHeight(p));
        p.height = "150";
        CHECK(!hasValidHeight(p));
        p.height = "149cm";
        CHECK(!hasValidHeight(p));
        p.height = "150cm";
        CHECK(hasValidHeight(p));
        p.height = "180cm";
        CHECK(hasValidHeight(p));
        p.height = "193cm";
        CHECK(hasValidHeight(p));
        p.height = "194cm";
        CHECK(!hasValidHeight(p));
        p.height = "59";
        CHECK(!hasValidHeight(p));
        p.height = "58in";
        CHECK(!hasValidHeight(p));
        p.height = "59in";
        CHECK(hasValidHeight(p));
        p.height = "60in";
        CHECK(hasValidHeight(p));
        p.height = "76in";
        CHECK(hasValidHeight(p));
        p.height = "77in";
        CHECK(!hasValidHeight(p));
    }

    SECTION("Valid Haircolor")
    {
        Passport p;
        CHECK(!hasValidHairColor(p));
        p.hairColor = "";
        CHECK(!hasValidHairColor(p));
        p.hairColor = "123456";
        CHECK(!hasValidHairColor(p));
        p.hairColor = "#123456";
        CHECK(hasValidHairColor(p));
        p.hairColor = "#1a2b3c";
        CHECK(hasValidHairColor(p));
        p.hairColor = "#9e8f0a";
        CHECK(hasValidHairColor(p));
        p.hairColor = "#9e8f0g";
        CHECK(!hasValidHairColor(p));
        p.hairColor = "#123abc";
        CHECK(hasValidHairColor(p));
        p.hairColor = "#123abz";
        CHECK(!hasValidHairColor(p));
        p.hairColor = "123abc";
        CHECK(!hasValidHairColor(p));
    }

    SECTION("Valid Eye Color")
    {
        Passport p;
        CHECK(!hasValidEyeColor(p));
        p.eyeColor = "";
        CHECK(!hasValidEyeColor(p));
        p.eyeColor = "123";
        CHECK(!hasValidEyeColor(p));
        p.eyeColor = "abcd";
        CHECK(!hasValidEyeColor(p));
        p.eyeColor = "amb";
        CHECK(hasValidEyeColor(p));
        p.eyeColor = "blu";
        CHECK(hasValidEyeColor(p));
        p.eyeColor = "brn";
        CHECK(hasValidEyeColor(p));
        p.eyeColor = "gry";
        CHECK(hasValidEyeColor(p));
        p.eyeColor = "grn";
        CHECK(hasValidEyeColor(p));
        p.eyeColor = "hzl";
        CHECK(hasValidEyeColor(p));
        p.eyeColor = "oth";
        CHECK(hasValidEyeColor(p));
        p.eyeColor = "amc";
        CHECK(!hasValidEyeColor(p));
    }

    SECTION("Valid Passport Id")
    {
        Passport p;
        CHECK(!hasValidPassportId(p));
        p.passportId = "";
        CHECK(!hasValidPassportId(p));
        p.passportId = "111";
        CHECK(!hasValidPassportId(p));
        p.passportId = "123456789";
        CHECK(hasValidPassportId(p));
        p.passportId = "000000000";
        CHECK(hasValidPassportId(p));
        p.passportId = "999999999";
        CHECK(hasValidPassportId(p));
        p.passportId = "111112222";
        CHECK(hasValidPassportId(p));
        p.passportId = "1a1111111";
        CHECK(!hasValidPassportId(p));
        p.passportId = "1234567890";
        CHECK(!hasValidPassportId(p));
    }

    char const invalid_passports_str[] =
        R"(eyr:1972 cid:100
hcl:#18171d ecl:amb hgt:170 pid:186cm iyr:2018 byr:1926

iyr:2019
hcl:#602927 eyr:1967 hgt:170cm
ecl:grn pid:012533040 byr:1946

hcl:dab227 iyr:2012
ecl:brn hgt:182cm pid:021572410 eyr:2020 byr:1992 cid:277

hgt:59cm ecl:zzz
eyr:2038 hcl:74454a iyr:2023
pid:3556412378 byr:2007)";
    auto const invalid_passports = parseInput(invalid_passports_str);

    char const valid_passports_str[] =
        R"(pid:087499704 hgt:74in ecl:grn iyr:2012 eyr:2030 byr:1980
hcl:#623a2f

eyr:2029 ecl:blu cid:129 byr:1989
iyr:2014 pid:896056539 hcl:#a97842 hgt:165cm

hcl:#888785
hgt:164cm byr:2001 iyr:2015 cid:88
pid:545766238 ecl:hzl
eyr:2022

iyr:2010 hgt:158cm hcl:#b6652a ecl:blu byr:1944 eyr:2021 pid:093154719)";
    auto const valid_passports = parseInput(valid_passports_str);

    SECTION("Passport Is Valid2")
    {
        CHECK(std::none_of(begin(invalid_passports), end(invalid_passports), isValid2));
        CHECK(std::all_of(begin(valid_passports), end(valid_passports), isValid2));
    }

    SECTION("Count Valid Passports 2")
    {
        CHECK(countValidPassports2(invalid_passports) == 0);
        CHECK(countValidPassports2(valid_passports) == 4);
    }
}
