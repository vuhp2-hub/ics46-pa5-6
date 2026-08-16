#include "HeapEntry.hpp"
#include "IndexedMinHeap.hpp"
#include "Meal.hpp"
#include "MenuModel.hpp"
#include "ModelIndex.hpp"
#include <gtest/gtest.h>
#include <vector>

namespace {
void fillUnorderedHeapNumTree(std::vector<HeapEntry>& data, std::vector<int> const& entries) {
    for (int entry : entries) {
        data.push_back(HeapEntry{static_cast<double>(entry), entry});
    }
}
} // namespace

// Ensure the macro evaluates properly as a string
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

TEST(MenuModelTests, ModelIndex) {
#ifdef PROJECT_ROOT_DIR
    std::string projectRoot = TOSTRING(PROJECT_ROOT_DIR);
    MenuModel model{projectRoot + "/starter/samples/menu3.menu"};
    std::vector<int> tablesExpected = {0, 1};
    EXPECT_EQ(model.tablesWith(0), tablesExpected);
#else
    ADD_FAILURE() << "PROJECT_ROOT_DIR not defined for MenuModelTests/ModelIndex";
#endif
}

TEST(IndexedMinHeapTests, Heapify) {
    std::vector<HeapEntry> raw;
    std::vector<int> entries{12, 9, 8, 7, 6, 4};
    fillUnorderedHeapNumTree(raw, entries);
    IndexedMinHeap::heapify(raw);

    std::vector<HeapEntry> expected;
    std::vector<int> expected_entries{4, 6, 8, 7, 9, 12};
    fillUnorderedHeapNumTree(expected, expected_entries);

    EXPECT_EQ(raw, expected);
}
