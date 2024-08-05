#include <gtest/gtest.h>
#include "../include/MasterNode.h"
#include "../include/FileUtils.h"
#include <vector>
#include <string>

class MasterNodeTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        FileUtils::loadEnv(".env");
    }

    void TearDown() override
    {
    }
};

TEST_F(MasterNodeTest, InitializeTaskQueue)
{
    MasterNode masterNode("config.ini");
    EXPECT_TRUE(masterNode.initializeTaskQueue(1000, 100));
    EXPECT_GT(masterNode.getRemainingTaskCount(), 0);
}

TEST_F(MasterNodeTest, GetNextTaskSuccess)
{
    MasterNode masterNode("config.ini");
    masterNode.initializeTaskQueue(1000, 100);

    grpc::ServerContext context;
    user_analysis::GetTaskRequest request;
    user_analysis::Task response;

    grpc::Status status = masterNode.GetTask(&context, &request, &response);

    EXPECT_TRUE(status.ok());
    EXPECT_FALSE(response.id().empty());
    EXPECT_GT(response.start_user_id(), 0);
    EXPECT_GT(response.end_user_id(), response.start_user_id());
}

TEST_F(MasterNodeTest, GetNextTaskEmptyQueue)
{
    MasterNode masterNode("config.ini");
    grpc::ServerContext context;
    user_analysis::GetTaskRequest request;
    user_analysis::Task response;

    grpc::Status status = masterNode.GetTask(&context, &request, &response);

    EXPECT_EQ(status.error_code(), grpc::StatusCode::NOT_FOUND);
    EXPECT_EQ(status.error_message(), "No tasks available");
}

TEST_F(MasterNodeTest, SubmitTaskResult)
{
    MasterNode masterNode("config.ini");
    masterNode.initializeTaskQueue(1000, 100);

    std::string validTaskId = masterNode.getValidTaskId();
    ASSERT_FALSE(validTaskId.empty()) << "No valid task ID available";

    grpc::ServerContext context;
    user_analysis::TaskResult result;
    result.set_task_id(validTaskId);
    auto *category_count = result.add_category_counts();
    category_count->set_category("Category A");
    category_count->set_count(100);

    user_analysis::SubmitResultResponse response;
    grpc::Status status = masterNode.SubmitResult(&context, &result, &response);

    EXPECT_TRUE(status.ok());
    EXPECT_TRUE(response.success());
}

TEST_F(MasterNodeTest, GetAggregatedResults)
{
    MasterNode masterNode("config.ini");
    masterNode.initializeTaskQueue(1000, 100);

    // Simulate submitting results for two different tasks
    for (int i = 0; i < 2; ++i)
    {
        std::string validTaskId = masterNode.getValidTaskId();
        ASSERT_FALSE(validTaskId.empty()) << "No valid task ID available";

        grpc::ServerContext context;
        user_analysis::TaskResult result;
        user_analysis::SubmitResultResponse response;

        result.set_task_id(validTaskId);
        auto *category_count1 = result.add_category_counts();
        category_count1->set_category("Category A");
        category_count1->set_count(100 + i * 50);
        auto *category_count2 = result.add_category_counts();
        category_count2->set_category(i == 0 ? "Category B" : "Category C");
        category_count2->set_count(80 + i * 40);

        EXPECT_TRUE((masterNode.SubmitResult(&context, &result, &response)).ok());
        EXPECT_TRUE(response.success());

        // Remove the submitted task ID to get a different one in the next iteration
        masterNode.removeTaskId(validTaskId);
    }

    auto aggregatedResults = masterNode.getAggregatedResults();

    EXPECT_EQ(aggregatedResults.size(), 3);
    EXPECT_EQ(aggregatedResults["Category A"], 250);
    EXPECT_EQ(aggregatedResults["Category B"], 80);
    EXPECT_EQ(aggregatedResults["Category C"], 120);
}

TEST_F(MasterNodeTest, SubmitInvalidTaskResult)
{
    MasterNode masterNode("config.ini");
    masterNode.initializeTaskQueue(1000, 100);

    grpc::ServerContext context;
    user_analysis::TaskResult result;
    user_analysis::SubmitResultResponse response;

    result.set_task_id("task_0");
    auto *category_count = result.add_category_counts();
    category_count->set_category("Category A");
    category_count->set_count(100);

    grpc::Status status = masterNode.SubmitResult(&context, &result, &response);

    // The MasterNode should return an error status
    EXPECT_FALSE(status.ok());
    EXPECT_EQ(status.error_code(), grpc::StatusCode::INVALID_ARGUMENT);
    EXPECT_FALSE(response.success());
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
