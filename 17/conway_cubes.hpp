#ifndef ADVENT_OF_CODE_17_CONWAY_CUBES_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_17_CONWAY_CUBES_HPP_INCLUDE_GUARD

#include <array>
#include <bitset>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

struct Vec4 {
    int x;
    int y;
    int z;
    int w;

    friend bool operator==(Vec4 const&, Vec4 const&) = default;
};

template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

struct HashVec4 {
    std::size_t operator()(Vec4 const& v) const {
        std::size_t seed = 0;
        hash_combine(seed, v.x);
        hash_combine(seed, v.y);
        hash_combine(seed, v.z);
        hash_combine(seed, v.w);
        return seed;
    }
};

struct World {
    Vec4 min;
    Vec4 max;
    std::unordered_set<Vec4, HashVec4> map;
};

World parseInput(std::string_view input);

int countNeighbours3(World const& w, Vec4 const& p);

World step3(World const& w);

int bootCycle3(World w);

int countNeighbours4(World const& w, Vec4 const& p);

World step4(World const& w);

int bootCycle4 (World w);

#endif
