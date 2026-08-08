// driver.cpp -- PROVIDED. Not graded. Loads the sample menu, exercises every piece you build, and
// prints what it finds -- run it as you work to watch each part come alive.
//
//   g++ -std=c++20 -Wall -Wextra -o driver driver.cpp && ./driver samples/menu3
//
// (The argument is the sample's base path: it reads <base>.menu, <base>.names, <base>.chosen.)
#include <iostream>
#include <string>
#include <vector>

#include "MenuModel.hpp"
#include "ModelIndex.hpp"
#include "Meal.hpp"
#include "MenuNames.hpp"
#include "mealScore.hpp"
#include "NameHash.hpp"
#include "MenuIndex.hpp"
#include "MealTweaker.hpp"
#include "BestMeals.hpp"

// Spread report for any NameMap, measured through the map's public bucket interface -- exactly the
// view the autograder uses.
template <class Map>
static void reportSpread(std::string const& label, Map const& map) {
    std::size_t maxBucket = 0;
    int alone = 0;
    for (std::size_t b = 0; b < map.bucket_count(); ++b)
        if (map.bucket_size(b) > maxBucket) maxBucket = map.bucket_size(b);
    for (auto const& kv : map)
        if (map.bucket_size(map.bucket(kv.first)) == 1) ++alone;
    std::cout << "  " << label << ": " << map.size() << " keys in " << map.bucket_count()
              << " buckets, biggest bucket " << maxBucket << ", keys alone ";
    if (map.empty()) std::cout << "-\n";
    else std::cout << (100 * alone / static_cast<int>(map.size())) << "%\n";
}

static std::string mealWords(Meal const& meal, MenuNames const& names) {
    std::string out = "(";
    for (int c = 0; c < meal.size(); ++c) {
        if (c > 0) out += ", ";
        out += (meal.dishFor(c) >= 0 && meal.dishFor(c) < names.numDishes(c))
                   ? "\"" + names.dishName(c, meal.dishFor(c)) + "\""
                   : std::string("?");
    }
    return out + ")";
}

int main(int argc, char* argv[]) {
    std::string const base = (argc > 1) ? argv[1] : "samples/menu3";
    MenuModel model(base + ".menu");
    model.readChosen(base + ".chosen");
    MenuNames names(base + ".names");

    std::cout << "menu: " << model.numCourses() << " courses, " << model.numTables()
              << " pairing tables, " << model.chosenCount() << " chosen course(s)\n\n";

    std::cout << "name maps and their spread:\n";
    NameDirectory dir(model, names);
    std::vector<std::string> courseKeys;
    std::vector<int> courseVals;
    for (int c = 0; c < model.numCourses(); ++c) {
        courseKeys.push_back(names.courseName(c));
        courseVals.push_back(c);
    }
    NameMap<std::string, int> courseMap = makeNameMap(courseKeys, courseVals);
    reportSpread("course-name map", courseMap);
    int big = 0;
    for (int c = 1; c < model.numCourses(); ++c) if (names.numDishes(c) > names.numDishes(big)) big = c;
    std::vector<std::string> dishKeys;
    std::vector<int> dishVals;
    for (int d = 0; d < names.numDishes(big); ++d) {
        dishKeys.push_back(names.dishName(big, d));
        dishVals.push_back(d);
    }
    NameMap<std::string, int> dishMap = makeNameMap(dishKeys, dishVals);
    reportSpread(names.courseName(big) + " dish map", dishMap);

    std::cout << "\nlookups:\n";
    std::cout << "  courseIndexOf(\"Main\") = " << dir.courseIndexOf("Main") << "\n";
    std::cout << "  dishIndexOf(courseIndexOf(\"Main\"), \"Lentils\") = "
              << dir.dishIndexOf(dir.courseIndexOf("Main"), "Lentils") << "\n";
    std::cout << "  courseIndexOf(\"Drink\") = " << dir.courseIndexOf("Drink") << "   (a miss -> -1)\n";

    std::cout << "\ntables by course (model.tablesWith):\n";
    for (int c = 0; c < model.numCourses(); ++c) {
        std::cout << "  " << names.courseName(c) << ":";
        for (int t : model.tablesWith(c)) std::cout << " table" << t;
        std::cout << "\n";
    }

    std::cout << "\ntweak session (start = every free course at dish 0):\n";
    Meal start(model.numCourses());
    for (int c = 0; c < model.numCourses(); ++c)
        start.setDish(c, model.isChosen(c) ? model.chosenDish(c) : 0);
    long long const touchesBefore = ics46::tableTouchCount;
    MealTweaker tweaker(model, names, start);
    struct Swap { char const* course; char const* dish; };
    Swap const script[] = {{"Appetizer", "Bread"}, {"Main", "Steak"},
                           {"Dessert", "Fruit"}, {"Appetizer", "Soup"}};
    std::cout << "  start " << mealWords(tweaker.currentMeal(), names)
              << "  score " << tweaker.score() << "\n";
    for (auto const& s : script) {
        bool const ok = tweaker.alterDish(s.course, s.dish);
        std::cout << "  swap \"" << s.course << "\" -> \"" << s.dish << "\": " << (ok ? "ok" : "refused")
                  << "  " << mealWords(tweaker.currentMeal(), names)
                  << "  score " << tweaker.score() << "\n";
    }
    long long const swapTouches = ics46::tableTouchCount - touchesBefore - model.numTables();
    std::cout << "  (self-check: full rescore of the final meal = "
              << mealScore(model, tweaker.currentMeal()) << ")\n";
    std::cout << "  table lookups spent by the swaps: " << swapTouches
              << " on top of the " << model.numTables() << " that filled the cache\n";

    std::cout << "\nbest meals (K = 3):\n";
    for (Meal const& meal : bestMeals(model, 3, 1000000000LL))
        std::cout << "  " << mealWords(meal, names) << "  score " << mealScore(model, meal) << "\n";

    std::cout << "\nsettling order: ";
    for (int c : settlingOrder(model)) std::cout << names.courseName(c) << " ";
    std::cout << "\n\ncounters: tableTouchCount=" << ics46::tableTouchCount
              << "  mealBuildCount=" << ics46::mealBuildCount
              << "  entryCompareCount=" << ics46::entryCompareCount
              << "  scoreCallCount=" << ics46::scoreCallCount << "\n";
    return 0;
}
