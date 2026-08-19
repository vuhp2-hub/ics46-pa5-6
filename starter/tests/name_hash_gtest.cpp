#include "NameHash.hpp"
#include <algorithm>
#include <gtest/gtest.h>

TEST(NameHasherTests, HashCodeIsOrderAware) {
    EXPECT_NE(NameHasher{}.hashCode("TAB"), NameHasher{}.hashCode("BAT"));
}

TEST(NameHasherTests, DifferentParametersCanChangeBuckets) {
    NameHasher first(10, IntegerHasher::a, IntegerHasher::b);
    NameHasher second(33, 1000000007ULL, 0ULL);

    EXPECT_NE(first("Pasta"), second("Pasta"));
}

TEST(NameHasherTests, SpaceConstraints) {
    EXPECT_LE(sizeof(NameHasher), 32);
}

TEST(NameHasherTests, MakeNameMapUsesExactBucketCount) {
    std::vector<std::string> keys{"TAB", "BAT", "ABT", "BTA", "TBA", "ATB"};
    std::vector<int> values{0, 1, 2, 3, 4, 5};

    auto map = makeNameMap(keys, values);

    EXPECT_EQ(map.bucket_count(), static_cast<std::size_t>(bucketCountFor(keys.size())));
    for (int i = 0; i < static_cast<int>(keys.size()); ++i) {
        EXPECT_EQ(map.at(keys[i]), values[i]);
    }
}

TEST(NameHasherTests, MakeNameMapImprovesAwkwardSpread) {
    std::vector<std::string> keys{
        "aaaaaaaa", "aaaaaaab", "aaaaaaac", "aaaaaaad",
        "aaaaaaae", "aaaaaaaf", "aaaaaaag", "aaaaaaah",
        "baaaaaaa", "caaaaaaa", "daaaaaaa", "eaaaaaaa",
    };
    std::vector<int> values;
    for (int i = 0; i < static_cast<int>(keys.size()); ++i) values.push_back(i);

    auto map = makeNameMap(keys, values);

    std::size_t biggest = 0;
    int alone = 0;
    for (std::size_t b = 0; b < map.bucket_count(); ++b) {
        biggest = std::max(biggest, map.bucket_size(b));
        if (map.bucket_size(b) == 1) ++alone;
    }

    EXPECT_LE(biggest, 2U);
    EXPECT_GE(alone, 8);
}
