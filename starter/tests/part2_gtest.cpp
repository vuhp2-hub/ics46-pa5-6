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
        data.push_back(HeapEntry{static_cast<double>(entry), static_cast<long long>(entry)});
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

TEST(IndexedMinHeapTests, HeapifyBuildFromLocator) {
    // Ensures locator keeps track of all entries
    // within the buildFrom method
    std::vector<HeapEntry> raw;
    std::vector<int> entries{12, 9, 8, 7, 6, 4};
    fillUnorderedHeapNumTree(raw, entries);

    IndexedMinHeap heap{};
    heap.buildFrom(raw);

    for (int entry : entries) {
        EXPECT_TRUE(heap.contains(static_cast<long long>(entry)));
    }
}

TEST(IndexedMinHeapTests, NewInsertion) {
    // New Insertion mainly tests the static helper _percolateUpIn
    // Invariant: buildFrom should be working.
    std::vector<HeapEntry> raw;
    std::vector<int> entries{4, 6, 8, 7, 9, 12};
    fillUnorderedHeapNumTree(raw, entries);

    IndexedMinHeap heap{};
    heap.buildFrom(raw);

    EXPECT_FALSE(heap.contains(5));
    heap.insert(5, 5);
    EXPECT_TRUE(heap.contains(5));

    std::vector<HeapEntry> expected_raw;
    std::vector<int> expected_entries{4, 6, 5, 7, 9, 12, 8};
    fillUnorderedHeapNumTree(expected_raw, expected_entries);

    IndexedMinHeap expected_heap{};
    expected_heap.buildFrom(expected_raw);

    EXPECT_EQ(heap, expected_heap);
}

TEST(IndexedMinHeapTests, AlreadyExistedInsertionPriorityLower) {
    // Invariant: buildFrom should be working.
    std::vector<HeapEntry> raw;
    std::vector<int> entries{4, 6, 5, 7, 9, 12, 8};
    fillUnorderedHeapNumTree(raw, entries);

    IndexedMinHeap heap{};
    heap.buildFrom(raw);

    heap.insert(13, 5);

    std::vector<HeapEntry> expected_raw;
    fillUnorderedHeapNumTree(expected_raw, (std::vector<int>{4, 6, 8, 7, 9, 12}));
    // Adding in heap entry id:5 manually for different priority
    expected_raw.push_back(HeapEntry{13, 5});

    IndexedMinHeap expected_heap{};
    expected_heap.buildFrom(expected_raw);

    EXPECT_EQ(heap, expected_heap);
}

TEST(IndexedMinHeapTests, AlreadyExistedInsertionPriorityHigher) {
    // Invariant: buildFrom should be working.
    std::vector<HeapEntry> raw;
    std::vector<int> entries{4, 6, 5, 7, 9, 12, 8};
    fillUnorderedHeapNumTree(raw, entries);

    IndexedMinHeap heap{};
    heap.buildFrom(raw);

    heap.insert(1, 5);

    std::vector<HeapEntry> expected_raw;
    expected_raw.push_back(HeapEntry{1, 5});
    fillUnorderedHeapNumTree(expected_raw, (std::vector<int>{6, 4, 7, 9, 12, 8}));

    IndexedMinHeap expected_heap{};
    expected_heap.buildFrom(expected_raw);

    EXPECT_EQ(heap, expected_heap);
}
