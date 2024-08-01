#include "../include/LogGenerator.h"
#include <chrono>
#include <random>
#include <algorithm>

std::vector<LogEntry> SimpleLogGenerator::generateLogs(int count)
{
    std::vector<LogEntry> logs;
    std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<> userDist(1, 1000);
    std::uniform_int_distribution<> queryDist(0, 4);
    std::vector<std::string> queries = {"shoes", "laptop", "book", "phone", "headphones"};

    for (int i = 0; i < count; i++)
    {
        LogEntry entry;
        entry.userId = userDist(rng);
        entry.searchQuery = queries[queryDist(rng)];
        entry.timestamp = generateTimestamp();
        entry.searchResults = generateMockResults();
        logs.push_back(entry);
    }
    return logs;
}

std::string SimpleLogGenerator::generateTimestamp()
{
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    char buf[100] = {0};
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now_c));
    return std::string(buf);
}

std::vector<std::string> SimpleLogGenerator::generateMockResults()
{
    std::mt19937 rng{std::random_device{}()};
    std::vector<std::string> results = {"Product A", "Product B", "Product C", "Product D", "Product E"};
    std::shuffle(results.begin(), results.end(), rng);
    results.resize(std::uniform_int_distribution<>{1, 5}(rng));
    return results;
}