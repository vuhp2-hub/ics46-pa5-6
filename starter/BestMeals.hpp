// BestMeals.hpp -- SUBMITTED, YOU IMPLEMENT. Return the K highest-scoring meals among the FIRST M
// meals this menu can still realize, best first.
//
// "Can still realize" means the meals that agree with the courses already chosen: a locked course
// keeps its dish in every one of them. Those meals come in a fixed order (see mealScan.hpp), and
// there can be an astronomical number of them, so you are asked for the best K out of the first M of
// them, never the whole space. M counts meals VISITED, so if the menu can realize fewer than M, you
// simply see them all. Return fewer than K only when fewer than K meals were seen.
//
// Walking the meals is PROVIDED: startingMeal() sets up the first one and advanceToNextMeal() steps
// to the next, so you visit them in order while holding only ONE working meal. What is left to you is
// the part that matters: deciding which K to KEEP as they stream past, using your IndexedMinHeap,
// while never holding more than K meals at once.
#ifndef BEST_MEALS_HPP
#define BEST_MEALS_HPP

#include <vector>

#include "IndexedMinHeap.hpp"
#include "Meal.hpp"
#include "MenuModel.hpp"
#include "mealScan.hpp"
#include "mealScore.hpp"

inline std::vector<Meal> bestMeals(MenuModel const& model, int K, long long M) {
    std::vector<Meal> result;
    if (K <= 0 || M <= 0)
        return result;

    // The scan is set up for you: `working` is the meal you score, `dish` is its odometer.
    std::vector<int> dish;
    Meal working(model.numCourses());

    startingMeal(model, dish, working);
    IndexedMinHeap heap{};
    long long seen = 0;
    do {
        double const score = mealScore(model, working);
        // TODO: keep the best K as they go by (IndexedMinHeap; never more than K meals stored)

        // We're using the variable seen as the id of the HeapEntry.

        if (heap.size() == K) {
            HeapEntry candidate{score, seen};
            if (heap.peekMin().outranks(candidate)) {
                heap.replaceMin(candidate);
            }
        } else {
            heap.insert(score, seen);
        }

        // We have to pop the worst Meal constantly.
        ++seen;
    } while (seen < M && advanceToNextMeal(model, dish, working));

    // TODO: return them best first

    // This is where the heapsort happens.
    /*
      Heap Sorting can be explained as popping the root and swap with the element at the end
      of the array.
      After the element  at the end of the array is swapped, it percolates down.
     */
    int i = heap.size() - 1;
    result.resize(i + 1);

    for (; i >= 0; --i) {
        // removeMin already popped the root, swap with maximum
        // and percolates down already.
        result[i] = *(MenuModel::MealIterator{&model, heap.removeMin().id});
    }

    return result;
}

#endif // BEST_MEALS_HPP
