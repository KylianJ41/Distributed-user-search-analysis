#pragma once

#include <string>
#include <filesystem>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <vector>

namespace FileUtils
{
    // Find a file in the current directory or one level up
    inline std::string findFile(const std::string &filename)
    {
        namespace fs = std::filesystem;
        std::vector<fs::path> searchPaths = {
            fs::current_path(),
            fs::current_path() / "..",
            fs::current_path() / ".." / "..",
            fs::current_path() / ".." / "UserAnalysis",
            fs::current_path() / "UserAnalysis"};

        for (const auto &basePath : searchPaths)
        {
            fs::path filePath = basePath / filename;
            if (fs::exists(filePath))
            {
                std::cout << "Found file: " << filePath << std::endl;
                return filePath.string();
            }
            else
            {
                std::cout << "File not found at: " << filePath << std::endl;
            }
        }

        throw std::runtime_error("Could not find file: " + filename);
    }

    inline void loadEnv(const std::string &filename)
    {
        try
        {
            std::string envPath = findFile(filename);
            std::ifstream file(envPath);
            if (!file.is_open())
            {
                throw std::runtime_error("Could not open .env file: " + envPath);
            }

            std::cout << "Loading .env file from: " << envPath << std::endl;

            std::string line;
            while (std::getline(file, line))
            {
                size_t pos = line.find('=');
                if (pos != std::string::npos)
                {
                    std::string key = line.substr(0, pos);
                    std::string value = line.substr(pos + 1);
                    setenv(key.c_str(), value.c_str(), 1);
                    std::cout << "Loaded environment variable: " << key << std::endl;
                }
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error in loadEnv: " << e.what() << std::endl;
            throw;
        }
    }

} // namespace FileUtils