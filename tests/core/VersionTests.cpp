#include "avs/core/Version.hpp"

#include <gtest/gtest.h>

TEST(CoreVersionTests, ReturnsProjectVersion)
{
    EXPECT_EQ(avs::core::version(), "0.1.0");
}
