#pragma once

#include <string>
#include <vector>

struct LogEntry
{
    int userId;
    std::string searchQuery;
    std::string timestamp;
    std::vector<std::string> searchResults;
};

class LogGenerator
{
public:
    virtual std::vector<LogEntry> generateLogs(int count) = 0;
    virtual ~LogGenerator() = default;
};

class SimpleLogGenerator : public LogGenerator
{
public:
    SimpleLogGenerator(int minUserId = 101, int maxUserId = 1000);
    std::vector<LogEntry> generateLogs(int count) override;

private:
    std::string generateTimestamp();
    std::vector<std::string> generateMockResults();
    int minUserId;
    int maxUserId;
};