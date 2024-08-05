#include "../../include/MasterNode.h"
#include <algorithm>

MasterNode::MasterNode(const std::string &configFile)
    : dbHandler(std::make_unique<DatabaseHandler>(configFile))
{
}

bool MasterNode::initializeTaskQueue(int totalUsers, int usersPerTask)
{
    std::lock_guard<std::mutex> lock(taskQueueMutex);
    for (int startUserId = 1; startUserId <= totalUsers; startUserId += usersPerTask)
    {
        int endUserId = std::min(startUserId + usersPerTask - 1, totalUsers);
        user_analysis::Task task;
        task.set_id("task_" + std::to_string(taskQueue.size() + 1));
        task.set_start_user_id(startUserId);
        task.set_end_user_id(endUserId);
        taskQueue.push(task);
    }
    return !taskQueue.empty();
}

grpc::Status MasterNode::GetTask(grpc::ServerContext *context,
                                 const user_analysis::GetTaskRequest *request,
                                 user_analysis::Task *response)
{
    std::lock_guard<std::mutex> lock(taskQueueMutex);
    if (taskQueue.empty())
    {
        return grpc::Status(grpc::StatusCode::NOT_FOUND, "No tasks available");
    }
    *response = taskQueue.front();
    taskQueue.pop();
    return grpc::Status::OK;
}

grpc::Status MasterNode::SubmitResult(grpc::ServerContext *context,
                                      const user_analysis::TaskResult *request,
                                      user_analysis::SubmitResultResponse *response)
{
    std::lock_guard<std::mutex> lock(taskResultsMutex);
    taskResults[request->task_id()] = std::vector<user_analysis::CategoryCount>(
        request->category_counts().begin(),
        request->category_counts().end());
    response->set_success(true);
    return grpc::Status::OK;
}

std::map<std::string, int> MasterNode::getAggregatedResults() const
{
    std::lock_guard<std::mutex> lock(taskResultsMutex);
    std::map<std::string, int> aggregatedResults;
    for (const auto &[taskId, categoryCounters] : taskResults)
    {
        for (const auto &counter : categoryCounters)
        {
            aggregatedResults[counter.category()] += counter.count();
        }
    }
    return aggregatedResults;
}

size_t MasterNode::getRemainingTaskCount() const
{
    std::lock_guard<std::mutex> lock(taskQueueMutex);
    return taskQueue.size();
}