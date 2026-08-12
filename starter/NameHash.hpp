// NameHash.hpp -- SUBMITTED. This starter is complete and working; you may change anything in this
// file. See the instructions for the requirements your submitted version must meet.
#ifndef NAME_HASH_HPP
#define NAME_HASH_HPP

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "IntegerHash.hpp"
#include "hashSupport.hpp"

// Getting from a name to a bucket takes TWO steps, as in class:
//
//   step 1  the HASH CODE     turn the key into a number          -- hashCode() below
//   step 2  the COMPRESSION   turn that number into a bucket      -- the map does this
//
// The map takes whatever operator() returns and reduces it with % bucket_count(), so a compression
// step of your own never needs to know how many buckets there are. If you add one that needs a
// modulus of its own, use HASH_PRIME from IntegerHash.hpp: a fixed prime far larger than any bucket
// count, which is exactly what such a scheme needs.
//
// Integer keys never come through this file: the menu model's own course-table index uses the
// provided IntegerHasher from IntegerHash.hpp, a complete worked example of these same two steps.
// String keys are yours.
struct NameHasher {

    // Step 1. Turn a name into a number.
    static std::uint32_t hashCode(std::string const& key) {
        std::uint32_t h = 0;
        for (char c : key) h += static_cast<std::uint32_t>(c);
        return h;
    }

    // Step 2. Compression. The map does this for you: it takes whatever operator() returns and
    // reduces it with % bucket_count() to pick the bucket.
    std::size_t operator()(std::string const& key) const { return hashCode(key); }
};

// Every name map carries YOUR NameHasher.
template <class K, class V>
using NameMap = std::unordered_map<K, V, NameHasher>;

// Build the map {keys[i] -> values[i]} with EXACTLY bucketCountFor(keys.size()) buckets. The keys are
// distinct; the bucket count never changes after this (the load factor stays below 1, so the map never
// rehashes).
template <class K, class V>
NameMap<K, V> makeNameMap(std::vector<K> const& keys, std::vector<V> const& values) {
    int const n = static_cast<int>(keys.size());
    NameMap<K, V> map(static_cast<std::size_t>(bucketCountFor(n)));
    for (int i = 0; i < n; ++i) map.emplace(keys[i], values[i]);
    return map;
}

#endif // NAME_HASH_HPP
