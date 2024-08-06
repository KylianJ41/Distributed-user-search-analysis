#pragma once

#include <grpcpp/grpcpp.h>
#include "user_analysis.grpc.pb.h"
#include "DatabaseHandler.h"
#include <string>
#include <map>
#include <memory>

class WorkerNode
{
public:
    WorkerNode(const std::string &master_address, std::unique_ptr<DatabaseHandler> db_handler);

    // Constructor for testing
    WorkerNode(std::shared_ptr<user_analysis::UserAnalysisService::StubInterface> stub,
               std::unique_ptr<DatabaseHandler> db_handler);

    void Run();

private:
    user_analysis::TaskResult ProcessTask(const user_analysis::Task &task);
    std::map<std::string, int> AnalyzeSearchData(int start_user_id, int end_user_id);

    std::shared_ptr<user_analysis::UserAnalysisService::StubInterface> stub_;
    std::unique_ptr<DatabaseHandler> db_handler_;
};