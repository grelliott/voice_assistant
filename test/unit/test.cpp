/*
 * Filename: /home/grant/src/test_c++/test/unit/test.cpp
 * Path: /home/grant/src/test_c++/test/unit
 * Created Date: Thursday, August 15th 2024, 3:25:45 pm
 * Author: Grant Elliott
 *
 * Copyright (c) 2024 Grant Elliott
 */

#include <gtest/gtest.h>

namespace ca::grantelliott::testapp::tests {
TEST(SanityTests, TestIsSane) {
    ASSERT_EQ(4, 2 + 2);
}

}  // namespace ca::grantelliott::testapp::tests
