// mealScore.hpp -- PROVIDED. A meal's TOTAL SCORE: multiply, over every pairing table, the one score for
// the dishes this meal picks on that table's courses. Higher is better.
//
// Every table lookup goes through tableScoreFor (see tableScore.hpp), so one full mealScore call costs
// exactly numTables() counted table touches. scoreCallCount counts whole-meal scorings the same
// tamper-proof way.
#ifndef MEAL_SCORE_HPP
#define MEAL_SCORE_HPP

#include "MenuModel.hpp"
#include "Meal.hpp"
#include "tableScore.hpp"

namespace ics46 { inline long long scoreCallCount = 0; }

inline double mealScore(MenuModel const& model, Meal const& meal) {
    ics46::scoreCallCount++;
    double score = 1.0;
    for (int t = 0; t < model.numTables(); ++t)
        score *= tableScoreFor(model, t, meal);
    return score;
}

#endif  // MEAL_SCORE_HPP
