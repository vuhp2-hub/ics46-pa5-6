// IntegerHash.hpp -- PROVIDED, complete and working. The hasher behind the menu model's own
// course-table index (see MenuModel.hpp / ModelIndex.hpp). Read it: it is a small worked example of
// the same two steps your NameHasher follows.
//
//   step 1  the HASH CODE     an integer already IS a number, so the key is its own code
//   step 2  the COMPRESSION   MAD: ((a * code + b) mod p), then the map's own % bucket_count() last
//
// Why not std::hash? On this platform std::hash on an integer is the IDENTITY function, no mixing at
// all (see hashSupport.hpp). MAD actually mixes: multiplying by a big constant and reducing mod a
// large prime scatters nearby keys far apart.
#ifndef INTEGER_HASH_HPP
#define INTEGER_HASH_HPP

#include <cstddef>
#include <cstdint>

// A fixed prime FAR LARGER than any bucket count, and (a * x) mod m without 64-bit overflow. A
// compression that multiplies by a large constant needs a modulus of its own: use HASH_PRIME for
// that, and let the map apply the final mod by its bucket count. Schemes that need neither can
// ignore both.
inline constexpr std::uint64_t HASH_PRIME = (1ULL << 61) - 1;
inline std::uint64_t mulmod(std::uint64_t a, std::uint64_t x, std::uint64_t m) {
    return static_cast<std::uint64_t>((static_cast<unsigned __int128>(a) * x) % m);
}

struct IntegerHasher {
    // Fixed MAD constants. Any large odd multiplier (and any addend) below HASH_PRIME serves;
    // nothing about these two is special.
    static constexpr std::uint64_t a = 1234567891234567891ULL;
    static constexpr std::uint64_t b = 987654321987654321ULL;

    std::size_t operator()(long long key) const {
        std::uint64_t const code = static_cast<std::uint64_t>(key);
        return static_cast<std::size_t>((mulmod(a, code, HASH_PRIME) + b) % HASH_PRIME);
    }
    std::size_t operator()(int key) const { return operator()(static_cast<long long>(key)); }
};

#endif  // INTEGER_HASH_HPP
