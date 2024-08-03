#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>
#include "../include/DatabaseHandler.h"
#include "../include/LogGenerator.h"
#include "../include/ConfigReader.h"
#include "../include/FileUtils.h"

void populateDatabase(DatabaseHandler &db, int numEntries)
{
    SimpleLogGenerator generator(101, 1000); // Use user IDs from 101 to 1000
    std::vector<LogEntry> logs = generator.generateLogs(numEntries);

    std::cout << "Generating " << numEntries << " log entries..." << std::endl;

    try
    {
        db.insertSearchLogs(logs);
        std::cout << "Successfully inserted " << numEntries << " log entries into the database." << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error inserting logs into database: " << e.what() << std::endl;
    }
}

void addXLogsToDatabase(int numEntries)
{
    try
    {
        std::string envPath = FileUtils::findFile(".env");
        FileUtils::loadEnv(envPath);

        std::cout << "Initializing DatabaseHandler..." << std::endl;
        DatabaseHandler db("config.ini");

        std::cout << "Creating search_logs table if it doesn't exist..." << std::endl;
        db.createSearchLogsTable();

        // Add 100 entries to the database
        populateDatabase(db, numEntries);

        std::cout << "Main logic completed successfully." << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error in main logic: " << e.what() << std::endl;
        throw; // Re-throw the exception to be caught in main()
    }
}

int main()
{

    return 0;
}