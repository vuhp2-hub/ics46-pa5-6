#include "NameHash.hpp"
#include <gtest/gtest.h>

TEST(HelloNameHash, Test) {
    EXPECT_EQ(NameHasher{}("TAB"), 9116);
    EXPECT_EQ(NameHasher{}("BAT"), 7334);
}
