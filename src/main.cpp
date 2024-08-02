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

int main()
{
    try
    {
        std::string envPath = FileUtils::findFile(".env");
        FileUtils::loadEnv(".env");

        std::string configPath = FileUtils::findFile("config.ini");
        std::cout << "Initializing DatabaseHandler..." << std::endl;
        DatabaseHandler db(configPath);

        int numEntries = 100;
        std::cout << "Populating database with " << numEntries << " entries..." << std::endl;
        populateDatabase(db, numEntries);

        std::cout << "Database initialization complete." << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error in main: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}