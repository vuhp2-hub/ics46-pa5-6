// MealTweaker.hpp -- SUBMITTED, and nearly finished: ONE line is left for you (see the TODO in
// alterDish). A tweak session: one current meal, a per-table score cache, and a running total, so a
// one-dish change re-scores only what it must.
#ifndef MEAL_TWEAKER_HPP
#define MEAL_TWEAKER_HPP

#include <string>
#include <vector>

#include "Meal.hpp"
#include "MenuIndex.hpp"
#include "MenuModel.hpp"
#include "MenuNames.hpp"
#include "tableScore.hpp"

class MealTweaker {
  private:
    MenuModel const& _model;
    NameDirectory _dir;
    Meal _meal;
    std::vector<double> _tableScore;
    double _score;

    void _recombine() {
        _score = 1.0;
        for (double s : _tableScore) _score *= s; // cached multiplies, zero-safe
    }

  public:
    MealTweaker(MenuModel const& model, MenuNames const& names, Meal startingMeal)
        : _model(model), _dir(model, names), _meal(static_cast<Meal&&>(startingMeal)),
          _tableScore(static_cast<std::size_t>(model.numTables())), _score(0.0) {
        for (int t = 0; t < _model.numTables(); ++t) {
            _tableScore[t] = tableScoreFor(_model, t, _meal);
        }
        _recombine();
    }

    double score() const { return _score; }

    Meal currentMeal() const { return _meal; }

    // The names given here always exist on this menu -- requests, like the menu files
    // themselves, are valid. A swap is refused (false, nothing changes) only for a locked
    // course or the already-selected dish.
    bool alterDish(std::string const& courseName, std::string const& dishName) {
        int const c = _dir.courseIndexOf(courseName);
        int const d = _dir.dishIndexOf(c, dishName);
        if (_model.isChosen(c))
            return false; // locked course
        if (_meal.dishFor(c) == d)
            return false; // already selected

        _meal.setDish(c, d);

        // TODO (one line). Changing course c's dish changes the score of SOME pairing tables and
        // leaves the rest untouched. Refresh exactly the ones it can affect -- the model's
        // tablesWith(c) serves the tables a course appears in -- by re-reading each one:
        std::vector<int> tablesWithCourse = _model.tablesWith(c);
        for (int t : tablesWithCourse) {
            _tableScore[t] = tableScoreFor(_model, t, _meal);
        }

        _recombine(); // rebuilds the total from the cache (a score can be 0, so we
        return true;  // never divide the old contribution back out)
    }
};

#endif // MEAL_TWEAKER_HPP
