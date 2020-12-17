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

struct Vec3 {
    int x;
    int y;
    int z;

    friend bool operator==(Vec3 const&, Vec3 const&) = default;
};

template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

struct HashVec3 {
    std::size_t operator()(Vec3 const& v) const {
        std::size_t seed = 0;
        hash_combine(seed, v.x);
        hash_combine(seed, v.y);
        hash_combine(seed, v.z);
        return seed;
    }
};

struct World {
    Vec3 min;
    Vec3 max;
    std::unordered_set<Vec3, HashVec3> map;
};

World parseInput(std::string_view input);

int countNeighbours(World const& w, Vec3 const& p);

World step(World const& w);

int bootCycle(World w);

#endif
