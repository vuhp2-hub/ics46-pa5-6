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
    std::uint64_t scale = 10;
    std::uint64_t a = IntegerHasher::a;
    std::uint64_t b = IntegerHasher::b;

    NameHasher() = default;
    NameHasher(std::uint64_t scale, std::uint64_t a, std::uint64_t b)
        : scale(scale), a(a), b(b) {}

    // Step 1. Turn a name into a number.
    static std::uint32_t hashCode(std::string const& key) {
        return hashCodeWithScale(key, 10);
    }

    static std::uint32_t hashCodeWithScale(std::string const& key, std::uint64_t scale) {
        std::uint32_t h = 0;
        for (unsigned char c : key) {
            h = static_cast<std::uint32_t>(h * scale + c);
        }
        return h;
    }

    // Step 2. Compression. The map does this for you: it takes whatever operator() returns and
    // reduces it with % bucket_count() to pick the bucket.

    // TODO: change the default compression algorithm to use the MAD compression function
    // like in IntegerHash.
    std::size_t operator()(std::string const& key) const {
        std::uint32_t const code = hashCodeWithScale(key, scale);
        return static_cast<std::size_t>((mulmod(a, code, HASH_PRIME) + b) % HASH_PRIME);
    }
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
    int const bucketCount = bucketCountFor(n);

    std::vector<NameHasher> candidates;
    for (std::uint64_t scale : {10ULL, 31ULL, 33ULL, 37ULL, 39ULL, 41ULL, 53ULL, 67ULL, 131ULL}) {
        for (std::uint64_t a : {
                 1234567891234567891ULL,
                 1000000007ULL,
                 1000000009ULL,
                 1469598103934665603ULL,
                 1099511628211ULL,
                 11400714819323198485ULL % HASH_PRIME,
             }) {
            for (std::uint64_t b : {
                     987654321987654321ULL,
                     0ULL,
                     271828182845904523ULL,
                     314159265358979323ULL,
                     112358132134558914ULL,
                 }) {
                candidates.emplace_back(scale, a, b);
            }
        }
    }

    NameHasher best;
    int bestBiggest = MAX_NAMES + 1;
    int bestAlone = -1;

    for (NameHasher const& candidate : candidates) {
        std::vector<int> bucketSizes(static_cast<std::size_t>(bucketCount), 0);
        for (K const& key : keys) {
            ++bucketSizes[candidate(key) % static_cast<std::size_t>(bucketCount)];
        }

        int biggest = 0;
        int alone = 0;
        for (int size : bucketSizes) {
            if (size > biggest) biggest = size;
            if (size == 1) ++alone;
        }

        if (biggest < bestBiggest || (biggest == bestBiggest && alone > bestAlone)) {
            best = candidate;
            bestBiggest = biggest;
            bestAlone = alone;
            if (bestBiggest == 1) break;
        }
    }

    NameMap<K, V> map(static_cast<std::size_t>(bucketCount), best);
    for (int i = 0; i < n; ++i) map.emplace(keys[i], values[i]);
    return map;
}

#endif // NAME_HASH_HPP
