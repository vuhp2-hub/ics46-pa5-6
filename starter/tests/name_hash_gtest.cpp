#include "NameHash.hpp"
#include <gtest/gtest.h>

TEST(NameHasherTests, HashCode) {
    EXPECT_EQ(NameHasher{}.hashCode("TAB"), 9116);
    EXPECT_EQ(NameHasher{}.hashCode("BAT"), 7334);
}

TEST(NameHasherTests, MADCompression) {
    EXPECT_EQ(NameHasher{}("TAB"), 488822844268373846ULL);
    EXPECT_EQ(NameHasher{}("BAT"), 263071454132421338ULL);
}

TEST(NameHasherTests, SpaceConstraints) {
    EXPECT_LE(sizeof(NameHasher), 32);
}
