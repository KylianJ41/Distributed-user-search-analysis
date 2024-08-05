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

    grpc::ServerContext context;
    user_analysis::GetTaskRequest getRequest;
    user_analysis::Task task;
    EXPECT_TRUE(masterNode.GetTask(&context, &getRequest, &task).ok());

    user_analysis::TaskResult result;
    result.set_task_id(task.id());
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

    // Simulate submitting results for two tasks
    {
        grpc::ServerContext context;
        user_analysis::TaskResult result;
        user_analysis::SubmitResultResponse response;

        result.set_task_id("task_1");
        auto *category_count1 = result.add_category_counts();
        category_count1->set_category("Category A");
        category_count1->set_count(100);
        auto *category_count2 = result.add_category_counts();
        category_count2->set_category("Category B");
        category_count2->set_count(80);

        EXPECT_TRUE((masterNode.SubmitResult(&context, &result, &response)).ok());
        EXPECT_TRUE(response.success());
    }

    {
        grpc::ServerContext context;
        user_analysis::TaskResult result;
        user_analysis::SubmitResultResponse response;

        result.set_task_id("task_2");
        auto *category_count1 = result.add_category_counts();
        category_count1->set_category("Category A");
        category_count1->set_count(150);
        auto *category_count2 = result.add_category_counts();
        category_count2->set_category("Category C");
        category_count2->set_count(120);

        EXPECT_TRUE(masterNode.SubmitResult(&context, &result, &response).ok());
        EXPECT_TRUE(response.success());
    }

    auto aggregatedResults = masterNode.getAggregatedResults();

    EXPECT_EQ(aggregatedResults.size(), 3);
    EXPECT_EQ(aggregatedResults["Category A"], 250);
    EXPECT_EQ(aggregatedResults["Category B"], 80);
    EXPECT_EQ(aggregatedResults["Category C"], 120);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
