// MenuIndex.hpp -- SUBMITTED, and nearly finished: ONE line is left for you (see the TODO in
// settlingOrder). The name lookups, every one a NameMap built by YOUR makeNameMap, and the settling
// order, driven by YOUR IndexedMinHeap.
#ifndef MENU_INDEX_HPP
#define MENU_INDEX_HPP

#include <string>
#include <vector>

#include "IndexedMinHeap.hpp"
#include "MenuModel.hpp"
#include "MenuNames.hpp"
#include "NameHash.hpp"
#include "hashSupport.hpp"

class NameDirectory {
  private:
    NameMap<std::string, int> _courses;             // course name -> courseIdx
    std::vector<NameMap<std::string, int>> _dishes; // _dishes[c]: dish name -> dish index

    static NameMap<std::string, int> _courseMap(MenuNames const& names) {
        std::vector<std::string> keys;
        std::vector<int> values;
        for (int c = 0; c < names.numCourses(); ++c) {
            keys.push_back(names.courseName(c));
            values.push_back(c);
        }
        return makeNameMap(keys, values);
    }

    static NameMap<std::string, int> _dishMap(MenuNames const& names, int c) {
        std::vector<std::string> keys;
        std::vector<int> values;
        for (int d = 0; d < names.numDishes(c); ++d) {
            keys.push_back(names.dishName(c, d));
            values.push_back(d);
        }
        return makeNameMap(keys, values);
    }

  public:
    NameDirectory(MenuModel const& model, MenuNames const& names) : _courses(_courseMap(names)) {
        (void)model; // built from the names alone; the model parameter mirrors the other constructors
        for (int c = 0; c < names.numCourses(); ++c) _dishes.push_back(_dishMap(names, c));
    }

    int courseIndexOf(std::string const& name) const {
        auto const found = _courses.find(name);
        return found == _courses.end() ? -1 : found->second;
    }

    int dishIndexOf(int courseIdx, std::string const& name) const {
        if (courseIdx < 0 || courseIdx >= static_cast<int>(_dishes.size()))
            return -1;
        auto const found = _dishes[courseIdx].find(name);
        return found == _dishes[courseIdx].end() ? -1 : found->second;
    }
};

inline std::vector<int> settlingOrder(MenuModel const& model) {
    int const C = model.numCourses();
    std::vector<std::vector<int>> const neighbors = courseNeighbors(model);

    IndexedMinHeap heap;
    for (int c = 0; c < C; ++c) heap.insert(static_cast<double>(neighbors[c].size()), c);

    std::vector<int> order;
    order.reserve(static_cast<std::size_t>(C));
    while (heap.size() > 0) {
        int const settled = static_cast<int>(heap.removeMin().id);
        order.push_back(settled);
        for (int nb : neighbors[settled])
            if (heap.contains(nb)) {
                // TODO (one line). This neighbour now has one fewer unsettled neighbour, so its
                // count in the heap is out of date. Your heap can change the value of an id that is
                // already in it -- use that here rather than removing and re-adding.
                heap.insert(heap.priorityOf(nb) - 1, nb);
            }
    }
    return order;
}

#endif // MENU_INDEX_HPP
