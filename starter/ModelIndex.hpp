// ModelIndex.hpp -- SUBMITTED, and nearly finished: ONE line is left for you (see the TODO).
//
// The menu model SERVES its own course-table index -- model.tablesWith(c) answers "which pairing
// tables involve course c?" -- but the code that BUILDS the index is yours. MenuModel.hpp only
// declares buildCourseTableIndex(); this file defines it, and the model calls it once at the end of
// loading a menu. Splitting the definition into this file is what lets you write this one piece of
// the model without touching the rest of it.
#ifndef MODEL_INDEX_HPP
#define MODEL_INDEX_HPP

#include "MenuModel.hpp"

inline void MenuModel::buildCourseTableIndex() {
    _tablesByCourse.clear();
    for (int t = 0; t < numTables(); ++t) {
        PairingFactor const& f = factorAt(t);
        for (int j = 0; j < f.scopeSize(); ++j) {
            // TODO (one line). Record that table t involves course f.courseAt(j): append t to that
            // course's list in _tablesByCourse.
        }
    }
}

#endif  // MODEL_INDEX_HPP
