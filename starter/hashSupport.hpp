// hashSupport.hpp -- PROVIDED. Small helpers for the name-map half of the PA.
//
// GOTCHA worth knowing: on this platform (libstdc++), std::hash on INTEGERS is the identity function --
// it does no mixing at all. std::hash<std::string> mixes well. That is why the model's course-table
// index uses the provided IntegerHasher (IntegerHash.hpp) instead.
#ifndef HASH_SUPPORT_HPP
#define HASH_SUPPORT_HPP

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "MenuModel.hpp"

// The bucket count your map must have when it is built: about 25% more buckets than keys, rounded to
// what the library actually uses (the next prime at or above the request). Your map's bucket_count()
// must EQUAL this value -- no more, no less -- and must never change afterward.
inline int bucketCountFor(int nKeys) {
    std::unordered_map<std::string, int> probe(static_cast<std::size_t>((nKeys * 5 + 3) / 4));
    return static_cast<int>(probe.bucket_count());
}

// No name map in this assignment is built from more than this many names, so its bucket_count()
// never exceeds MAX_BUCKET_COUNT. (HASH_PRIME, far larger than either, lives in IntegerHash.hpp.)
inline constexpr int MAX_NAMES = 512;
inline constexpr int MAX_BUCKET_COUNT = 1024;   // bucketCountFor(MAX_NAMES) is 661, well under this

// courseNeighbors(model)[c] = the distinct OTHER courses that share at least one pairing table with
// course c (each neighbor listed once, in ascending order).
inline std::vector<std::vector<int>> courseNeighbors(MenuModel const& model) {
    int const C = model.numCourses();
    std::vector<std::vector<int>> neighbors(C);
    std::vector<std::vector<bool>> seen(C, std::vector<bool>(C, false));
    for (int t = 0; t < model.numTables(); ++t) {
        PairingFactor const& f = model.factorAt(t);
        for (int i = 0; i < f.scopeSize(); ++i)
            for (int j = 0; j < f.scopeSize(); ++j) {
                int a = f.courseAt(i), b = f.courseAt(j);
                if (a != b && !seen[a][b]) { seen[a][b] = true; neighbors[a].push_back(b); }
            }
    }
    return neighbors;   // scopes are ascending, so each neighbor list comes out ascending
}

#endif  // HASH_SUPPORT_HPP
