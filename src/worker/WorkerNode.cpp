#include "../include/WorkerNode.h"
#include "../include/TaskInterface.h"
#include <iostream>

WorkerNode::WorkerNode(const std::string &master_address, std::unique_ptr<DatabaseHandler> db_handler)
    : db_handler_(std::move(db_handler))
{
    auto channel = grpc::CreateChannel(master_address, grpc::InsecureChannelCredentials());
    auto stub = user_analysis::UserAnalysisService::NewStub(channel);
    task_interface_ = std::make_unique<GrpcTaskInterface>(std::move(stub));
}

// Constructor for testing
WorkerNode::WorkerNode(std::unique_ptr<TaskInterface> task_interface,
                       std::unique_ptr<DatabaseHandler> db_handler)
    : task_interface_(std::move(task_interface)),
      db_handler_(std::move(db_handler)) {}

void WorkerNode::Run()
{
    while (true)
    {
        grpc::ClientContext context;
        user_analysis::GetTaskRequest request;
        user_analysis::Task task;

        auto status = task_interface_->GetTask(&context, request, &task);

        if (!status.ok())
        {
            if (status.error_code() == grpc::StatusCode::NOT_FOUND)
            {
                std::cout << "No more tasks available. Exiting." << std::endl;
            }
            else
            {
                std::cerr << "Error getting task: " << status.error_message() << std::endl;
            }
            break;
        }

        auto result = ProcessTask(task);

        grpc::ClientContext submit_context;
        user_analysis::SubmitResultResponse response;
        status = task_interface_->SubmitResult(&submit_context, result, &response);

        if (!status.ok())
        {
            std::cerr << "Error submitting result: " << status.error_message() << std::endl;
            break;
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