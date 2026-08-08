// tableScore.hpp -- PROVIDED. Look up ONE pairing table's score for a meal (the row whose dishes match
// the meal's picks on that table's courses).
//
// The counter below is bumped on every such lookup. The grading reads it to measure how much TABLE WORK a
// solution does over a workload: an organization that touches only the tables that changed does far fewer
// lookups than one that re-reads every table. It lives in this PROVIDED header (which a submission cannot
// override or edit) so it is tamper-proof. It never changes any answer.
#ifndef TABLE_SCORE_HPP
#define TABLE_SCORE_HPP

#include "MenuModel.hpp"
#include "Meal.hpp"

namespace ics46 { inline long long tableTouchCount = 0; }

inline double tableScoreFor(MenuModel const& model, int t, Meal const& meal) {
    ics46::tableTouchCount++;
    PairingFactor const& f = model.factorAt(t);
    int idx = 0;
    for (int j = 0; j < f.scopeSize(); ++j) {
        int c = f.courseAt(j);
        idx = idx * model.dishes(c) + meal.dishFor(c);
    }
    return f.scoreAt(idx);
}

#endif  // TABLE_SCORE_HPP
