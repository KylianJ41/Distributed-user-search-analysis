#include "../../include/MasterNode.h"
#include <algorithm>

MasterNode::MasterNode(const std::string &configFile)
    : dbHandler(std::make_unique<DatabaseHandler>(configFile))
{
}

bool MasterNode::initializeTaskQueue(int totalUsers, int usersPerTask)
{
    std::lock_guard<std::mutex> lock(taskQueueMutex);
    std::lock_guard<std::mutex> idLock(createdTaskIdsMutex);

    taskQueue = std::queue<user_analysis::Task>();
    createdTaskIds.clear();

    std::cout << "Initializing task queue with " << totalUsers << " total users and " << usersPerTask << " users per task" << std::endl;

    for (int startUserId = 1; startUserId <= totalUsers; startUserId += usersPerTask)
    {
        int endUserId = std::min(startUserId + usersPerTask - 1, totalUsers);
        user_analysis::Task task;
        std::string taskId = "task_" + std::to_string(taskQueue.size() + 1);
        task.set_id(taskId);
        task.set_start_user_id(startUserId);
        task.set_end_user_id(endUserId);
        taskQueue.push(task);
        createdTaskIds.insert(taskId);

        std::cout << "Created task with ID: " << taskId << std::endl;
    }

    std::cout << "Finished initializing task queue. Total tasks created: " << createdTaskIds.size() << std::endl;

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
    std::lock_guard<std::mutex> idLock(createdTaskIdsMutex);

    std::cout << "Submitting result for task ID: " << request->task_id() << std::endl;
    std::cout << "Number of created task IDs: " << createdTaskIds.size() << std::endl;

    // Check if the task ID is valid (it should be in our set of created task IDs)
    if (createdTaskIds.find(request->task_id()) == createdTaskIds.end())
    {
        std::cout << "Task ID not found in createdTaskIds" << std::endl;
        response->set_success(false);
        return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Invalid task ID: Task does not exist");
    }

    // Store the result
    taskResults[request->task_id()] = std::vector<user_analysis::CategoryCount>(
        request->category_counts().begin(),
        request->category_counts().end());
    response->set_success(true);
    std::cout << "Task result submitted successfully" << std::endl;
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

std::string MasterNode::getValidTaskId() const
{
    std::lock_guard<std::mutex> lock(createdTaskIdsMutex);
    if (!createdTaskIds.empty())
    {
        return *createdTaskIds.begin();
    }
    return "";
}

void MasterNode::removeTaskId(const std::string &taskId)
{
    std::lock_guard<std::mutex> lock(createdTaskIdsMutex);
    createdTaskIds.erase(taskId);
}