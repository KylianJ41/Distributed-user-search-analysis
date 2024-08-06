#include "../include/WorkerNode.h"
#include <iostream>

WorkerNode::WorkerNode(const std::string &master_address, std::unique_ptr<DatabaseHandler> db_handler)
    : db_handler_(std::move(db_handler))
{
    auto channel = grpc::CreateChannel(master_address, grpc::InsecureChannelCredentials());
    stub_ = user_analysis::UserAnalysisService::NewStub(channel);
}

WorkerNode::WorkerNode(std::shared_ptr<user_analysis::UserAnalysisService::StubInterface> stub,
                       std::unique_ptr<DatabaseHandler> db_handler)
    : stub_(std::move(stub)), db_handler_(std::move(db_handler))
{
}

void WorkerNode::Run()
{
    while (true)
    {
        grpc::ClientContext context;
        user_analysis::GetTaskRequest request;
        user_analysis::Task task;

        auto status = stub_->GetTask(&context, request, &task);

        if (status.ok())
        {
            auto result = ProcessTask(task);

            grpc::ClientContext submit_context;
            user_analysis::SubmitResultResponse response;
            stub_->SubmitResult(&submit_context, result, &response);
        }
        else if (status.error_code() == grpc::StatusCode::NOT_FOUND)
        {
            std::cout << "No more tasks available. Exiting." << std::endl;
            break;
        }
        else
        {
            std::cerr << "Error: " << status.error_message() << std::endl;
        }
    }
}

user_analysis::TaskResult WorkerNode::ProcessTask(const user_analysis::Task &task)
{
    auto category_counts = AnalyzeSearchData(task.start_user_id(), task.end_user_id());

    user_analysis::TaskResult result;
    result.set_task_id(task.id());
    for (const auto &[category, count] : category_counts)
    {
        auto *category_count = result.add_category_counts();
        category_count->set_category(category);
        category_count->set_count(count);
    }

    return result;
}

std::map<std::string, int> WorkerNode::AnalyzeSearchData(int start_user_id, int end_user_id)
{
    std::string query = "SELECT search_query FROM search_logs WHERE user_id BETWEEN " +
                        std::to_string(start_user_id) + " AND " + std::to_string(end_user_id);

    auto result = db_handler_->executeQuery(query);
    std::map<std::string, int> category_counts;
    for (const auto &row : result)
    {
        std::string category = row[0].as<std::string>();
        category_counts[category]++;
    }

    return category_counts;
}