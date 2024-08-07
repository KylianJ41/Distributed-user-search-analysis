#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/WorkerNode.h"
#include "../include/DatabaseHandler.h"
#include "../include/FileUtils.h"
#include "../include/TaskInterface.h"

class MockTaskInterface : public TaskInterface
{
public:
    MOCK_METHOD3(GetTask, grpc::Status(grpc::ClientContext *, const user_analysis::GetTaskRequest &, user_analysis::Task *));
    MOCK_METHOD3(SubmitResult, grpc::Status(grpc::ClientContext *, const user_analysis::TaskResult &, user_analysis::SubmitResultResponse *));
};

class MockDatabaseHandler : public DatabaseHandler
{
public:
    MockDatabaseHandler(const std::string &configFilename) : DatabaseHandler(configFilename) {}

    MOCK_METHOD1(executeQuery, pqxx::result(const std::string &));
};

class WorkerNodeTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        FileUtils::loadEnv(".env");
        mock_task_interface_ = std::make_unique<MockTaskInterface>();
        mock_db_handler_ = std::make_unique<MockDatabaseHandler>("config.ini");
    }

    std::unique_ptr<MockTaskInterface> mock_task_interface_;
    std::unique_ptr<MockDatabaseHandler> mock_db_handler_;
};

TEST_F(WorkerNodeTest, ProcessSingleTaskTest)
{
    user_analysis::Task task;
    task.set_id("test_task_1");
    task.set_start_user_id(1);
    task.set_end_user_id(100);

    EXPECT_CALL(*mock_task_interface_, GetTask(testing::_, testing::_, testing::_))
        .WillOnce(testing::DoAll(
            testing::SetArgPointee<2>(task),
            testing::Return(grpc::Status::OK)))
        .WillOnce(testing::Return(grpc::Status(grpc::StatusCode::NOT_FOUND, "")));

    EXPECT_CALL(*mock_db_handler_, executeQuery(testing::_))
        .WillOnce(testing::Return(pqxx::result{}));

    EXPECT_CALL(*mock_task_interface_, SubmitResult(testing::_, testing::_, testing::_))
        .WillOnce(testing::Return(grpc::Status::OK));

    {
        WorkerNode worker(std::move(mock_task_interface_), std::move(mock_db_handler_));
        worker.Run();
    }

    mock_task_interface_ = std::make_unique<MockTaskInterface>();
    mock_db_handler_ = std::make_unique<MockDatabaseHandler>("config.ini");
}

TEST_F(WorkerNodeTest, ProcessMultipleTasksTest)
{
    user_analysis::Task task1, task2;
    task1.set_id("test_task_1");
    task1.set_start_user_id(1);
    task1.set_end_user_id(50);
    task2.set_id("test_task_2");
    task2.set_start_user_id(51);
    task2.set_end_user_id(100);

    EXPECT_CALL(*mock_task_interface_, GetTask(testing::_, testing::_, testing::_))
        .WillOnce(testing::DoAll(
            testing::SetArgPointee<2>(task1),
            testing::Return(grpc::Status::OK)))
        .WillOnce(testing::DoAll(
            testing::SetArgPointee<2>(task2),
            testing::Return(grpc::Status::OK)))
        .WillOnce(testing::Return(grpc::Status(grpc::StatusCode::NOT_FOUND, "")));

    EXPECT_CALL(*mock_db_handler_, executeQuery(testing::_))
        .WillOnce(testing::Return(pqxx::result{}))
        .WillOnce(testing::Return(pqxx::result{}));

    EXPECT_CALL(*mock_task_interface_, SubmitResult(testing::_, testing::_, testing::_))
        .WillOnce(testing::Return(grpc::Status::OK))
        .WillOnce(testing::Return(grpc::Status::OK));

    WorkerNode worker(std::move(mock_task_interface_), std::move(mock_db_handler_));
    worker.Run();
}

TEST_F(WorkerNodeTest, GetTaskErrorTest)
{
    EXPECT_CALL(*mock_task_interface_, GetTask(testing::_, testing::_, testing::_))
        .WillOnce(testing::Return(grpc::Status(grpc::StatusCode::INTERNAL, "Internal error")));

    WorkerNode worker(std::move(mock_task_interface_), std::move(mock_db_handler_));
    worker.Run();
}

TEST_F(WorkerNodeTest, ExecuteQueryErrorTest)
{
    user_analysis::Task task;
    task.set_id("test_task_1");
    task.set_start_user_id(1);
    task.set_end_user_id(100);

    EXPECT_CALL(*mock_task_interface_, GetTask(testing::_, testing::_, testing::_))
        .WillOnce(testing::DoAll(
            testing::SetArgPointee<2>(task),
            testing::Return(grpc::Status::OK)))
        .WillOnce(testing::Return(grpc::Status(grpc::StatusCode::NOT_FOUND, "")));

    EXPECT_CALL(*mock_db_handler_, executeQuery(testing::_))
        .WillOnce(testing::Throw(std::runtime_error("Database error")));

    WorkerNode worker(std::move(mock_task_interface_), std::move(mock_db_handler_));
    worker.Run();
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}