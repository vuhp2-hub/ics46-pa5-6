// MenuNames.hpp -- PROVIDED. The names sidecar: every course's name and its dishes' names, read from a
// .names file. Names are single tokens (underscores stand in for spaces). Course order matches the .menu.
#ifndef MENU_NAMES_HPP
#define MENU_NAMES_HPP

#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

class MenuNames {
    private:
        std::vector<std::string>               _courseNames;   // _courseNames[c]
        std::vector<std::vector<std::string>>  _dishNames;     // _dishNames[c][d]

    public:
        MenuNames() = default;

        explicit MenuNames(std::string const& namesPath) {
            std::ifstream in(namesPath);
            if (!in) throw std::runtime_error("cannot open " + namesPath);

            std::string tag;
            in >> tag;                       // "NAMES" -- a format marker; skip it

            int numCourses = 0;
            if (!(in >> numCourses)) throw std::runtime_error("bad .names header");
            _courseNames.resize(numCourses);
            _dishNames.resize(numCourses);
            for (int c = 0; c < numCourses; ++c) {
                int numDishes = 0;
                in >> _courseNames[c] >> numDishes;
                _dishNames[c].resize(numDishes);
                for (int d = 0; d < numDishes; ++d) in >> _dishNames[c][d];
            }
            if (!in) throw std::runtime_error("bad .names body in " + namesPath);
        }

        int numCourses() const { return static_cast<int>(_courseNames.size()); }
        int numDishes(int courseIdx) const { return static_cast<int>(_dishNames[courseIdx].size()); }
        std::string const& courseName(int courseIdx) const { return _courseNames[courseIdx]; }
        std::string const& dishName(int courseIdx, int dish) const { return _dishNames[courseIdx][dish]; }
};

#endif  // MENU_NAMES_HPP
