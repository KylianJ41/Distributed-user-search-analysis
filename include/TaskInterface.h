#pragma once

#include "WorkerNode.h"
#include <grpcpp/grpcpp.h>
#include "user_analysis.grpc.pb.h"
#include <memory>

class TaskInterface
{
public:
    // ~ is a destructor, which is called when the object is destroyed
    // default means the default destructor, which is the destructor that the compiler generates
    virtual ~TaskInterface() = default;

    // = 0 mean pure virtual function, so the derived class must implement this function
    virtual grpc::Status GetTask(grpc::ClientContext *context,
                                 const user_analysis::GetTaskRequest &request,
                                 user_analysis::Task *response) = 0;

    virtual grpc::Status SubmitResult(grpc::ClientContext *context,
                                      const user_analysis::TaskResult &result,
                                      user_analysis::SubmitResultResponse *response) = 0;
};

class GrpcTaskInterface : public TaskInterface
{
public:
    explicit GrpcTaskInterface(std::shared_ptr<user_analysis::UserAnalysisService::Stub> stub)
        : stub_(std::move(stub)) {}

    grpc::Status GetTask(grpc::ClientContext *context,
                         const user_analysis::GetTaskRequest &request,
                         user_analysis::Task *response) override
    {
        return stub_->GetTask(context, request, response);
    }

    grpc::Status SubmitResult(grpc::ClientContext *context,
                              const user_analysis::TaskResult &result,
                              user_analysis::SubmitResultResponse *response) override
    {
        return stub_->SubmitResult(context, result, response);
    }

private:
    std::shared_ptr<user_analysis::UserAnalysisService::Stub> stub_;
};