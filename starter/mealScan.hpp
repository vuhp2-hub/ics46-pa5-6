// mealScan.hpp -- PROVIDED. Walking the complete meals, one at a time.
//
// The number of complete meals is the product of the free courses' dish counts, so it can be ENORMOUS
// -- far too many to store, and too many to number with one integer. These two helpers let you visit
// every complete meal exactly once while holding only ONE meal at a time:
//
//   startingMeal(model, dish, meal)      set up the first meal (locked courses pinned to their dish)
//   advanceToNextMeal(model, dish, meal) step to the next one; false when there are no more
//
// `dish` is one current-dish index per course. Locked courses never move.
#ifndef MEAL_SCAN_HPP
#define MEAL_SCAN_HPP

#include <vector>

#include "MenuModel.hpp"
#include "Meal.hpp"

inline void startingMeal(MenuModel const& model, std::vector<int>& dish, Meal& meal) {
    int const C = model.numCourses();
    dish.assign(static_cast<std::size_t>(C), 0);
    for (int c = 0; c < C; ++c) {
        dish[c] = model.isChosen(c) ? model.chosenDish(c) : 0;
        meal.setDish(c, dish[c]);
    }
}

// Odometer step: the last free course moves fastest; when it runs out of dishes it resets to 0 and
// the next course to its left moves. Returns false once every combination has been visited.
inline bool advanceToNextMeal(MenuModel const& model, std::vector<int>& dish, Meal& meal) {
    for (int c = model.numCourses() - 1; c >= 0; --c) {
        if (model.isChosen(c)) continue;
        if (dish[c] + 1 < model.dishes(c)) {
            ++dish[c];
            meal.setDish(c, dish[c]);
            return true;
        }
        dish[c] = 0;
        meal.setDish(c, 0);
    }
    return false;
}

#endif  // MEAL_SCAN_HPP
