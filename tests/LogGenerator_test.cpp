#include <gtest/gtest.h>
#include "../include/LogGenerator.h"

TEST(LogGeneratorTest, GeneratesCorrectNumberOfLogs)
{
    SimpleLogGenerator generator;
    int expectedCount = 5;

    std::vector<LogEntry> logs = generator.generateLogs(expectedCount);

    EXPECT_EQ(logs.size(), expectedCount);

    for (const auto &log : logs)
    {
        EXPECT_GT(log.userId, 0); // User ID should be greater than 0
        EXPECT_FALSE(log.searchQuery.empty());
        EXPECT_FALSE(log.timestamp.empty());
        EXPECT_FALSE(log.searchResults.empty());
    }
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}