#include "../include/DatabaseHandler.h"
#include "../include/FileUtils.h"
#include <gtest/gtest.h>
#include <pqxx/pqxx>
#include <fstream>

// We will use a reserved range of user IDs for testing
// The data put in DB in this range will also be removed at the end of the tests
const int TEST_USER_ID_START = 1;
const int TEST_USER_ID_END = 100;

class DatabaseHandlerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        try
        {
            std::cout << "Setting up test environment" << std::endl;
            std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

            // Load environment variables from .env file
            FileUtils::loadEnv(".env");
            std::cout << ".env file loaded successfully" << std::endl;

            std::cout << "Creating DatabaseHandler" << std::endl;
            handler = std::make_unique<DatabaseHandler>("config.ini");
            std::cout << "DatabaseHandler created successfully" << std::endl;

            if (!handler->isConnected())
            {
                throw std::runtime_error("Database connection is not valid");
            }

            std::cout << "Creating search logs table" << std::endl;
            handler->createSearchLogsTable();

            // Clean up any existing test data
            cleanupTestData();

            std::cout << "Setup complete" << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error in SetUp: " << e.what() << std::endl;
            throw;
        }
    }

    void TearDown() override
    {
        cleanupTestData();
    }

    void cleanupTestData()
    {
        try
        {
            std::cout << "Cleaning up test data" << std::endl;
            std::string cleanup_query = "DELETE FROM search_logs WHERE user_id BETWEEN " +
                                        std::to_string(TEST_USER_ID_START) + " AND " +
                                        std::to_string(TEST_USER_ID_END);
            handler->executeQuery(cleanup_query);
            std::cout << "Test data cleaned up" << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error in cleanupTestData: " << e.what() << std::endl;
        }
    }

    std::unique_ptr<DatabaseHandler> handler;
};

TEST_F(DatabaseHandlerTest, InsertAndRetrieveLogs)
{
    try
    {
        std::cout << "Starting InsertAndRetrieveLogs test" << std::endl;

        // Prepare test data using IDs from the test range
        std::vector<LogEntry> testLogs = {
            {TEST_USER_ID_START, "test query 1", "2023-01-01 12:00:00", {"result1", "result2"}},
            {TEST_USER_ID_START + 1, "test query 2", "2023-01-02 13:00:00", {"result3", "result4"}}};

        // Log test data
        std::cout << "Test logs:" << std::endl;
        for (const auto &log : testLogs)
        {
            std::cout << "User ID: " << log.userId << std::endl;
            std::cout << "Search Query: " << log.searchQuery << std::endl;
            std::cout << "Timestamp: " << log.timestamp << std::endl;
            std::cout << "Search Results: ";
            for (const auto &result : log.searchResults)
            {
                std::cout << result << " ";
            }
            std::cout << std::endl;
        }

        // Insert logs
        std::cout << "Inserting test logs" << std::endl;
        handler->insertSearchLogs(testLogs);

        // Retrieve logs
        std::cout << "Retrieving inserted logs" << std::endl;
        pqxx::result r = handler->executeQuery("SELECT * FROM search_logs WHERE user_id BETWEEN " +
                                               std::to_string(TEST_USER_ID_START) + " AND " +
                                               std::to_string(TEST_USER_ID_END) +
                                               " ORDER BY user_id");

        // Check results
        std::cout << "Asserting results" << std::endl;
        ASSERT_EQ(r.size(), 2);

        // Check first log entry
        EXPECT_EQ(r[0][1].as<int>(), TEST_USER_ID_START);
        EXPECT_EQ(r[0][2].as<std::string>(), "test query 1");
        EXPECT_EQ(r[0][3].as<std::string>(), "2023-01-01 12:00:00");
        std::string results1 = r[0][4].as<std::string>();
        // EXPECT_NE will return 0 if the string is found
        EXPECT_NE(results1.find("result1"), std::string::npos);
        EXPECT_NE(results1.find("result2"), std::string::npos);

        // Check second log entry
        EXPECT_EQ(r[1][1].as<int>(), TEST_USER_ID_START + 1);
        EXPECT_EQ(r[1][2].as<std::string>(), "test query 2");
        EXPECT_EQ(r[1][3].as<std::string>(), "2023-01-02 13:00:00");
        std::string results2 = r[1][4].as<std::string>();
        EXPECT_NE(results2.find("result3"), std::string::npos);
        EXPECT_NE(results2.find("result4"), std::string::npos);

        std::cout << "InsertAndRetrieveLogs test completed successfully" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error in InsertAndRetrieveLogs test: " << e.what() << std::endl;
        FAIL() << "Exception thrown: " << e.what();
    }
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