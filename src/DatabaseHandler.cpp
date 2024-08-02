#include "../include/DatabaseHandler.h"
#include "../include/ConfigReader.h"
#include <sstream>
#include <stdexcept>

DatabaseHandler::DatabaseHandler(const std::string &configPath)
{
    try
    {
        std::cout << "Initializing DatabaseHandler with config: " << configPath << std::endl;
        ConfigReader config(configPath);
        std::string connString =
            "host=" + config.get("database", "host") +
            " port=" + config.get("database", "port") +
            " dbname=" + config.get("database", "dbname") +
            " user=" + config.get("database", "user") +
            " password=" + config.get("database", "password");

        std::cout << "Connecting with: " << connString << std::endl;

        conn = std::make_unique<pqxx::connection>(connString);

        if (!conn)
        {
            throw std::runtime_error("Failed to create database connection");
        }

        if (!conn->is_open())
        {
            throw std::runtime_error("Database connection is not open");
        }

        std::cout << "Connected successfully" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error in DatabaseHandler constructor: " << e.what() << std::endl;
        throw;
    }
}

void DatabaseHandler::createTable(const std::string &tableName, const std::vector<std::string> &columns)
{
    try
    {
        std::cout << "Creating table: " << tableName << std::endl;
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
        std::cout << "Table created successfully" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error in createTable: " << e.what() << std::endl;
        throw;
    }
}

std::string DatabaseHandler::vectorToPostgresArray(const std::vector<std::string> &vec)
{
    try
    {
        std::cout << "Converting vector to Postgres array" << std::endl;
        std::string result = "{";
        for (size_t i = 0; i < vec.size(); ++i)
        {
            if (i > 0)
                result += ",";
            std::string escaped = vec[i];
            size_t pos = 0;
            while ((pos = escaped.find("\"", pos)) != std::string::npos)
            {
                escaped.replace(pos, 1, "\\\"");
                pos += 2;
            }
            result += "\"" + escaped + "\"";
        }
        result += "}";
        std::cout << "Converted array: " << result << std::endl;
        return result;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error in vectorToPostgresArray: " << e.what() << std::endl;
        throw;
    }
}

void DatabaseHandler::insertSearchLog(const LogEntry &log)
{
    try
    {
        if (!conn || !conn->is_open())
        {
            throw std::runtime_error("Database connection is not valid");
        }

        std::cout << "Inserting search log" << std::endl;
        std::cout << "User ID: " << log.userId << std::endl;
        std::cout << "Search Query: " << log.searchQuery << std::endl;
        std::cout << "Timestamp: " << log.timestamp << std::endl;
        std::cout << "Number of search results: " << log.searchResults.size() << std::endl;

        std::string arrayStr = vectorToPostgresArray(log.searchResults);
        std::cout << "Converted search results array: " << arrayStr << std::endl;

        std::string query = "INSERT INTO search_logs (user_id, search_query, timestamp, search_results) VALUES ($1, $2, $3, $4)";

        std::cout << "Creating transaction" << std::endl;
        pqxx::work txn(*conn);
        std::cout << "Transaction created" << std::endl;

        std::cout << "Executing query" << std::endl;
        txn.exec_params(query,
                        log.userId,
                        log.searchQuery,
                        log.timestamp,
                        arrayStr);
        std::cout << "Query executed" << std::endl;

        std::cout << "Committing transaction" << std::endl;
        txn.commit();
        std::cout << "Transaction committed" << std::endl;

        std::cout << "Search log inserted successfully" << std::endl;
    }
    catch (const pqxx::sql_error &e)
    {
        std::cerr << "SQL error in insertSearchLog: " << e.what() << std::endl;
        std::cerr << "Query that caused the error: " << e.query() << std::endl;
        throw;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error in insertSearchLog: " << e.what() << std::endl;
        throw;
    }
}

void DatabaseHandler::insertSearchLogs(const std::vector<LogEntry> &logs)
{
    try
    {
        std::cout << "Inserting multiple search logs" << std::endl;
        for (const auto &log : logs)
        {
            insertSearchLog(log);
        }
        std::cout << "All search logs inserted successfully" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error in insertSearchLogs: " << e.what() << std::endl;
        throw;
    }
}

pqxx::result DatabaseHandler::executeQuery(const std::string &query)
{
    try
    {
        std::cout << "Executing query: " << query << std::endl;
        pqxx::work txn(*conn);
        pqxx::result r = txn.exec(query);
        txn.commit();
        std::cout << "Query executed successfully" << std::endl;
        return r;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error in executeQuery: " << e.what() << std::endl;
        throw;
    }
}

void DatabaseHandler::createSearchLogsTable()
{
    createTable("search_logs", {"id SERIAL PRIMARY KEY",
                                "user_id INTEGER",
                                "search_query TEXT",
                                "timestamp TIMESTAMP",
                                "search_results TEXT[]"});
}