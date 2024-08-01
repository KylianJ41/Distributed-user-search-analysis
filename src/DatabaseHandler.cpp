#include "../include/DatabaseHandler.h"
#include "../include/ConfigReader.h"
#include <sstream>

DatabaseHandler::DatabaseHandler(const std::string &configPath)
{
    ConfigReader config(configPath);
    std::string connString =
        "host=" + config.get("database", "host") +
        " port=" + config.get("database", "port") +
        " dbname=" + config.get("database", "dbname") +
        " user=" + config.get("database", "user") +
        " password=" + config.get("database", "password");

    conn = std::make_unique<pqxx::connection>(connString);
}

void DatabaseHandler::createTable(const std::string &tableName, const std::vector<std::string> &columns)
{
    std::string query = "CREATE TABLE IF NOT EXISTS " + tableName + " (";
    for (size_t i = 0; i < columns.size(); i++)
    {
        query += columns[i];
        if (i < columns.size() - 1)
            query += ", ";
    }
    query += ")";

    pqxx::work txn(*conn);
    txn.exec(query);
    txn.commit();
}

std::string DatabaseHandler::vectorToPostgresArray(const std::vector<std::string> &vec)
{
    std::string result = "{";
    for (size_t i = 0; i < vec.size(); ++i)
    {
        if (i > 0)
            result += ",";
        std::string escaped = vec[i];
        // Replace any " with \"
        size_t pos = 0;
        while ((pos = escaped.find("\"", pos)) != std::string::npos)
        {
            escaped.replace(pos, 1, "\\\"");
            pos += 2;
        }
        result += "\"" + escaped + "\"";
    }
    result += "}";
    return result;
}

void DatabaseHandler::insertSearchLog(const LogEntry &log)
{
    std::string query = "INSERT INTO search_logs (user_id, search_query, timestamp, search_results) VALUES ($1, $2, $3, $4::text[])";

    pqxx::work txn(*conn);
    txn.exec_params(query,
                    log.userId,
                    log.searchQuery,
                    log.timestamp,
                    vectorToPostgresArray(log.searchResults));
    txn.commit();
}

void DatabaseHandler::insertSearchLogs(const std::vector<LogEntry> &logs)
{
    for (const auto &log : logs)
    {
        insertSearchLog(log);
    }
}

pqxx::result DatabaseHandler::executeQuery(const std::string &query)
{
    pqxx::work txn(*conn);
    pqxx::result r = txn.exec(query);
    txn.commit();
    return r;
}

void DatabaseHandler::createSearchLogsTable()
{
    createTable("search_logs", {"id SERIAL PRIMARY KEY",
                                "user_id INTEGER",
                                "search_query TEXT",
                                "timestamp TIMESTAMP",
                                "search_results TEXT[]"});
}