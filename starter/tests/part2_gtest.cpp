#include "MenuModel.hpp"
#include "ModelIndex.hpp"
#include <gtest/gtest.h>
#include <vector>

// Ensure the macro evaluates properly as a string
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

TEST(MenuModelTests, ModelIndex) {
#ifdef PROJECT_ROOT_DIR
    std::string projectRoot = TOSTRING(PROJECT_ROOT_DIR);
    MenuModel model{projectRoot + "/starter/samples/menu3.menu"};
    std::vector<int> tablesExpected = {0, 1};
    EXPECT_EQ(model.tablesWith(0), tablesExpected);
#else
    ADD_FAILURE() << "PROJECT_ROOT_DIR not defined for MenuModelTests/ModelIndex";
#endif
}
