// MinHeap.hpp -- PROVIDED, complete and working. Read and adapt it; do not submit it.
//
// A plain array-backed min-heap of HeapEntry, stored in level order: index i has children 2i+1 and
// 2i+2 and parent (i-1)/2. The root (index 0) is the minimum. All ordering goes through
// HeapEntry::outranks.
#ifndef MIN_HEAP_HPP
#define MIN_HEAP_HPP

#include <stdexcept>
#include <utility>   // std::swap
#include <vector>

#include "HeapEntry.hpp"

class MinHeap {
    private:
        std::vector<HeapEntry> _data;

        void _percolateUp(int i) {
            while (i > 0) {
                int parent = (i - 1) / 2;
                if (!_data[i].outranks(_data[parent])) break;
                std::swap(_data[i], _data[parent]);
                i = parent;
            }
        }

        void _percolateDown(int i) {
            int const n = static_cast<int>(_data.size());
            while (true) {
                int best = i;
                for (int c = 2 * i + 1; c <= 2 * i + 2 && c < n; ++c)
                    if (_data[c].outranks(_data[best])) best = c;
                if (best == i) break;
                std::swap(_data[i], _data[best]);
                i = best;
            }
        }

    public:
        // Sink the entry at slot i into place within data[0..n). Static, so it works on any level-order
        // array of entries, not just this heap's own.
        static void percolateDownIn(std::vector<HeapEntry>& data, int i) {
            int const n = static_cast<int>(data.size());
            while (true) {
                int best = i;
                for (int c = 2 * i + 1; c <= 2 * i + 2 && c < n; ++c)
                    if (data[c].outranks(data[best])) best = c;
                if (best == i) break;
                std::swap(data[i], data[best]);
                i = best;
            }
        }

        // Turn ANY array of entries into a heap, in place: the second half of the array is already a
        // pile of one-entry heaps, so start at the last entry that has a child and sink each one.
        static void heapify(std::vector<HeapEntry>& data) {
            for (int i = static_cast<int>(data.size()) / 2 - 1; i >= 0; --i) percolateDownIn(data, i);
        }

        // Replace this heap's contents with a whole batch at once, using heapify.
        void buildFrom(std::vector<HeapEntry> const& entries) {
            _data = entries;
            heapify(_data);
        }

        int size() const { return static_cast<int>(_data.size()); }

        HeapEntry const& peekMin() const {
            if (_data.empty()) throw std::out_of_range("peekMin: empty heap");
            return _data[0];
        }

        void insert(HeapEntry entry) {
            _data.push_back(entry);
            _percolateUp(static_cast<int>(_data.size()) - 1);
        }

        HeapEntry removeMin() {
            if (_data.empty()) throw std::out_of_range("removeMin: empty heap");
            HeapEntry top = _data[0];
            _data[0] = _data.back();
            _data.pop_back();
            if (!_data.empty()) _percolateDown(0);
            return top;
        }
};

#endif  // MIN_HEAP_HPP
