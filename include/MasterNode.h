#pragma once

#include <string>
#include <vector>
#include <queue>
#include <map>
#include <memory>
#include <mutex>
#include <grpcpp/grpcpp.h>
#include "user_analysis.grpc.pb.h"
#include "../include/DatabaseHandler.h"

class MasterNode final : public user_analysis::UserAnalysisService::Service
{
public:
    MasterNode(const std::string &configFile);

    grpc::Status GetTask(grpc::ServerContext *context,
                         const user_analysis::GetTaskRequest *request,
                         user_analysis::Task *response) override;

    grpc::Status SubmitResult(grpc::ServerContext *context,
                              const user_analysis::TaskResult *request,
                              user_analysis::SubmitResultResponse *response) override;

    bool initializeTaskQueue(int totalUsers, int usersPerTask);
    std::map<std::string, int> getAggregatedResults() const;
    size_t getRemainingTaskCount() const;

private:
    std::unique_ptr<DatabaseHandler> dbHandler;
    std::queue<user_analysis::Task> taskQueue;
    std::map<std::string, std::vector<user_analysis::CategoryCount>> taskResults;

    mutable std::mutex taskQueueMutex;
    mutable std::mutex taskResultsMutex;
};