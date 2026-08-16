// HeapEntry.hpp -- PROVIDED. Do not edit. One heap entry: a priority and a unique id.
//
// ALL ordering decisions between two entries must go through outranks() -- never compare the fields
// directly. The counter below measures how much ordering work a heap does; it lives in this PROVIDED
// header so it is tamper-proof, and it never changes any answer.
#ifndef HEAP_ENTRY_HPP
#define HEAP_ENTRY_HPP

#ifdef NOT_GRADESCOPE
#include <iostream>
#endif

namespace ics46 {
inline long long entryCompareCount = 0;
}

struct HeapEntry {
    double priority;
    long long id;

    // True when this entry should sit ABOVE other in a MIN-heap
    // (smaller priority wins; equal priorities break ties by smaller id).
    bool outranks(HeapEntry const& other) const {
        ++ics46::entryCompareCount;
        if (priority != other.priority)
            return priority < other.priority;
        return id < other.id;
    }

#ifdef NOT_GRADESCOPE
    // Equal comparison operator for my tests to work
    bool operator==(HeapEntry const& other) const { return other.id == id && other.priority == priority; }
    friend std::ostream& operator<<(std::ostream& out, HeapEntry const& entry) {
        out << "[" << entry.priority << ", " << entry.id << "]";
        return out;
    }
#endif
};

#endif // HEAP_ENTRY_HPP
