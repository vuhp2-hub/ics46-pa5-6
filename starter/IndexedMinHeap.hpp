// IndexedMinHeap.hpp -- SUBMITTED, YOU IMPLEMENT. Adapt the provided MinHeap (read MinHeap.hpp) into an
// INDEXED min-heap: alongside the level-order array it keeps a locator (id -> current slot in the
// array), so any entry can be found, inspected, or re-prioritized quickly by its id.
//
// RULE: every ordering decision between two entries goes through HeapEntry::outranks() -- never
// compare priority fields directly.
#ifndef INDEXED_MIN_HEAP_HPP
#define INDEXED_MIN_HEAP_HPP

#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "HeapEntry.hpp"

class IndexedMinHeap {
  private:
    std::vector<HeapEntry> _data;                // the heap, level order
    std::unordered_map<long long, int> _locator; // id -> its slot in _data

    // TODO: private helpers as you see fit (start from MinHeap's _percolateUp/_percolateDown --
    // what extra bookkeeping does every swap now owe?)

    // I guess two percolate up functions is needed because it would simplify one of them internally modifying locator.
  public:
    // Turn ANY array of entries into a heap, in place -- the same job as MinHeap::heapify, and
    // pure array work: it knows nothing about the locator. Whoever calls it owns putting the
    // locator right afterward.
    static void heapify(std::vector<HeapEntry>& data) {
        (void)data;
        // TODO
    }

    // ---- PROVIDED read-only accessors. They work as soon as YOUR code keeps _data and
    //      _locator correct, which is the real job below.
    int size() const { return static_cast<int>(_data.size()); }

    bool contains(long long id) const { return _locator.find(id) != _locator.end(); }

    double priorityOf(long long id) const { // throws std::out_of_range if id is absent
        auto const found = _locator.find(id);
        if (found == _locator.end())
            throw std::out_of_range("priorityOf: id is not in the heap");
        return _data[found->second].priority;
    }

    HeapEntry const& peekMin() const { // PROVIDED
        if (_data.empty())
            throw std::out_of_range("peekMin: empty heap");
        return _data[0];
    }

    // Add (priority, id). If id is ALREADY in the heap, CHANGE its priority to the given value
    // instead; the entry moves to where it now belongs.
    void insert(double priority, long long id) {
        (void)priority;
        (void)id;
        // TODO
    }

    HeapEntry removeMin() { // throws std::out_of_range if empty
        throw std::out_of_range("removeMin: TODO");
    }

    // Replace the minimum entry with this one (heap must be non-empty).
    void replaceMin(HeapEntry entry) {
        (void)entry;
        // TODO
    }

    // Replace the heap's ENTIRE contents with this batch of entries (ids distinct) -- whatever
    // was in the heap before is gone, locator included. After it returns, the heap holds exactly
    // these entries, every operation above works as usual, and the locator says where every id
    // ended up.
    void buildFrom(std::vector<HeapEntry> const& entries) {
        (void)entries;
        // TODO
    }
};

#endif // INDEXED_MIN_HEAP_HPP
