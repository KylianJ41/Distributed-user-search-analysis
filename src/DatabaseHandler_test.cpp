#include <gtest/gtest.h>
#include "../include/DatabaseHandler.h"
#include <pqxx/pqxx>
#include <fstream>

class DatabaseHandlerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        handler = std::make_unique<DatabaseHandler>("../config.ini");
        handler->createSearchLogsTable();
    }

    void TearDown() override
    {
        handler->executeQuery("DROP TABLE IF EXISTS search_logs");
        handler->executeQuery("DROP TABLE IF EXISTS test_table");
    }

    std::unique_ptr<DatabaseHandler> handler;
};

TEST_F(DatabaseHandlerTest, InsertAndRetrieveLogs)
{
    std::vector<LogEntry> testLogs = {
        {1, "test query 1", "2023-01-01 12:00:00", {"result1", "result2"}},
        {2, "test query 2", "2023-01-02 13:00:00", {"result3", "result4"}}};

    handler->insertSearchLogs(testLogs);

    pqxx::result r = handler->executeQuery("SELECT * FROM search_logs ORDER BY user_id");
    ASSERT_EQ(r.size(), 2);
    EXPECT_EQ(r[0][1].as<int>(), 1);
    EXPECT_EQ(r[0][2].as<std::string>(), "test query 1");
    EXPECT_EQ(r[0][3].as<std::string>(), "2023-01-01 12:00:00");
    EXPECT_EQ(r[1][1].as<int>(), 2);
    EXPECT_EQ(r[1][2].as<std::string>(), "test query 2");
    EXPECT_EQ(r[1][3].as<std::string>(), "2023-01-02 13:00:00");
}

TEST_F(DatabaseHandlerTest, CreateTableIdempotent)
{
    EXPECT_NO_THROW(handler->createSearchLogsTable());
    EXPECT_NO_THROW(handler->createSearchLogsTable());
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}