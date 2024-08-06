#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/WorkerNode.h"
#include "../include/DatabaseHandler.h"
#include "../include/FileUtils.h"

class MockUserAnalysisStub : public user_analysis::UserAnalysisService::StubInterface
{
public:
    MOCK_METHOD(grpc::Status, GetTask, (grpc::ClientContext * context, const user_analysis::GetTaskRequest &request, user_analysis::Task *response), (override));
    MOCK_METHOD(grpc::Status, SubmitResult, (grpc::ClientContext * context, const user_analysis::TaskResult &request, user_analysis::SubmitResultResponse *response), (override));

    MOCK_METHOD(grpc::ClientAsyncResponseReaderInterface<user_analysis::Task> *, AsyncGetTaskRaw, (grpc::ClientContext * context, const user_analysis::GetTaskRequest &request, grpc::CompletionQueue *cq), (override));
    MOCK_METHOD(grpc::ClientAsyncResponseReaderInterface<user_analysis::Task> *, PrepareAsyncGetTaskRaw, (grpc::ClientContext * context, const user_analysis::GetTaskRequest &request, grpc::CompletionQueue *cq), (override));
    MOCK_METHOD(grpc::ClientAsyncResponseReaderInterface<user_analysis::SubmitResultResponse> *, AsyncSubmitResultRaw, (grpc::ClientContext * context, const user_analysis::TaskResult &request, grpc::CompletionQueue *cq), (override));
    MOCK_METHOD(grpc::ClientAsyncResponseReaderInterface<user_analysis::SubmitResultResponse> *, PrepareAsyncSubmitResultRaw, (grpc::ClientContext * context, const user_analysis::TaskResult &request, grpc::CompletionQueue *cq), (override));
};

class WorkerNodeTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        FileUtils::loadEnv(".env");
        mock_stub_ = std::make_shared<MockUserAnalysisStub>();
        db_handler_ = std::make_unique<DatabaseHandler>("config.ini");
    }

    std::shared_ptr<MockUserAnalysisStub> mock_stub_;
    std::unique_ptr<DatabaseHandler> db_handler_;
};

TEST_F(WorkerNodeTest, ProcessTaskTest)
{
    user_analysis::Task task;
    task.set_id("test_task_1");
    task.set_start_user_id(1); // Use the predefined range for test values
    task.set_end_user_id(100); // Adjust this based on your predefined test range

    WorkerNode worker(mock_stub_, std::move(db_handler_));

    EXPECT_CALL(*mock_stub_, GetTask(testing::_, testing::_, testing::_))
        .WillOnce(testing::DoAll(
            testing::SetArgPointee<2>(task),
            testing::Return(grpc::Status::OK)))
        .WillOnce(testing::Return(grpc::Status(grpc::StatusCode::NOT_FOUND, "")));

    EXPECT_CALL(*mock_stub_, SubmitResult(testing::_, testing::_, testing::_))
        .WillOnce(testing::Return(grpc::Status::OK));

    worker.Run();

    testing::Mock::VerifyAndClearExpectations(mock_stub_.get());
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}