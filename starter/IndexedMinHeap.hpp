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

    // I'll just a single percolateDownIn function.

    static void _percolateDownIn(std::vector<HeapEntry>& data, int i, size_t arraySize,
                                 std::unordered_map<long long, int> *locator = nullptr) {
        // Percolation down takes O(Log N)
        // Have to examine all children but will disregard sub-tree of one child every
        // iteration
        while (true) {
            int best = i;
            for (int c = 2 * i + 1; c <= 2 * i + 2 && c < static_cast<int>(arraySize); ++c) {
                if (data[c].outranks(data[best])) {
                    best = c;
                }
            }
            if (best == i) {
                break;
            }

            if (locator != nullptr) {
                locator->operator[](data[i].id) = best;
                locator->operator[](data[best].id) = i;
            }
            std::swap(data[i], data[best]);
            // Percolate down more if needed.
            i = best;
        }
    }

    static void _percolateUpIn(std::vector<HeapEntry>& data, int i,
                               std::unordered_map<long long, int> *locator = nullptr) {
        while (i > 0) {
            int parentIndex = (i - 1) / 2;
            if (data[i].outranks(data[parentIndex])) {
                if (locator != nullptr) {
                    locator->operator[](data[i].id) = parentIndex;
                    locator->operator[](data[parentIndex].id) = i;
                }
                std::swap(data[i], data[parentIndex]);
                i = parentIndex;
            } else {
                break;
            }
        }
    }

  public:
    // Turn ANY array of entries into a heap, in place -- the same job as MinHeap::heapify, and
    // pure array work: it knows nothing about the locator. Whoever calls it owns putting the
    // locator right afterward.
    static void heapify(std::vector<HeapEntry>& data) {
        size_t arraySize = data.size();
        for (int i = (static_cast<int>(arraySize) - 1) / 2; i >= 0; --i) {
            _percolateDownIn(data, i, arraySize);
        }
    }

    // ---- PROVIDED read-only accessors. They work as soon as YOUR code keeps _data and
    //      _locator correct, which is the real job below.
    int size() const { return static_cast<int>(_data.size()); }

    bool contains(long long id) const { return _locator.find(id) != _locator.end(); }

    double priorityOf(long long id) const { // throws std::out_of_range if id is absent
        auto const found = _locator.find(id);
        if (found == _locator.end())
            throw std::out_of_range("priorityOf: id is not in the heap");
        int dataSize = _data.size();
        int second = found->second;
        int first = found->first;
        std::vector<HeapEntry> logData = _data;
        std::unordered_map<long long, int> logLocator = _locator;
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
        // TODO

        // Two cases: already in the heap and not in the heap

        // If not in the heap: add it in and percolate up
        // If already in the heap I can compare its new priority to the old priority
        // Then percolate up or down whether the new priority is less than or greater than the
        // old priority

        if (contains(id)) {
            int slot = _locator.at(id);
            double oldPriority = _data[slot].priority;
            _data[slot].priority = priority;
            if (priority > oldPriority) {
                _percolateDownIn(_data, slot, _data.size(), &_locator);
            } else {
                _percolateUpIn(_data, slot, &_locator);
            }

        } else {
            _locator[id] = _data.size();
            _data.push_back(HeapEntry{priority, id});
            _percolateUpIn(_data, static_cast<int>(_data.size()) - 1, &_locator);
        }
    }

    HeapEntry removeMin() { // throws std::out_of_range if empty
        if (_data.empty()) {
            throw std::out_of_range("Heap is empty");
        }

        HeapEntry root = _data[0];
        _data[0] = _data.back();
        // Gets rid of actual root.
        _data.pop_back();
        _percolateDownIn(_data, 0, _data.size(), &_locator);
        _locator.erase(root.id);

        if (size() == 1) {
            _locator[_data[0].id] -= 1;
        }

        return root;
    }

    // Replace the minimum entry with this one (heap must be non-empty).
    void replaceMin(HeapEntry entry) {
        // TODO
        if (_data.empty()) {
            throw std::out_of_range("Heap is empty");
        }

        _locator.erase(_data[0].id);
        _data[0] = entry;
        _locator[entry.id] = 0;
        _percolateDownIn(_data, 0, _data.size(), &_locator);
    }

    // Replace the heap's ENTIRE contents with this batch of entries (ids distinct) -- whatever
    // was in the heap before is gone, locator included. After it returns, the heap holds exactly
    // these entries, every operation above works as usual, and the locator says where every id
    // ended up.
    void buildFrom(std::vector<HeapEntry> const& entries) {
        // TODO
        _data = entries;
        heapify(_data);

        // heapify does not enforce the locator to keep track of all the entries
        // The code below will update the locator
        _locator.clear();
        for (size_t i = 0; i < _data.size(); ++i) {
            _locator[_data[i].id] = i;
        }
    }

#ifdef NOT_GRADESCOPE
    // Added equivalence operator for testing
    bool operator==(IndexedMinHeap const& other) const { return other._data == _data && other._locator == _locator; }
    friend std::ostream& operator<<(std::ostream& out, IndexedMinHeap const& heap) {
        out << "_data = {";

        for (auto const& entry : heap._data) {
            out << entry << ", ";
        }

        out << "}";

        out << "_locator = {";

        for (auto const& [key, value] : heap._locator) {
            out << "[" << key << "," << value << "]" << ", ";
        }
        out << "}";

        return out;
    }
#endif
};

#endif // INDEXED_MIN_HEAP_HPP
