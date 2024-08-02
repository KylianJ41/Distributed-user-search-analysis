#pragma once

#include <pqxx/pqxx>
#include "LogGenerator.h"
#include <string>
#include <vector>
#include <iostream>

class DatabaseHandler
{
public:
    DatabaseHandler(const std::string &configPath = "config.ini");
    void createTable(const std::string &tableName, const std::vector<std::string> &columns);
    void insertSearchLog(const LogEntry &log);
    void insertSearchLogs(const std::vector<LogEntry> &logs);
    pqxx::result executeQuery(const std::string &query);
    void createSearchLogsTable();

    bool isConnected() const { return conn && conn->is_open(); }

private:
    std::unique_ptr<pqxx::connection> conn;
    std::string vectorToPostgresArray(const std::vector<std::string> &vec);
};