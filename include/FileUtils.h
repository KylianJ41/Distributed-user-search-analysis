#pragma once

#include <string>
#include <filesystem>
#include <stdexcept>
#include <fstream>
#include <iostream>

namespace FileUtils
{
    // Find a file in the current directory or one level up
    inline std::string findFile(const std::string &filename)
    {
        namespace fs = std::filesystem;
        fs::path currentPath = fs::current_path();
        fs::path filePath = currentPath / filename;

        if (fs::exists(filePath))
        {
            return filePath.string();
        }

        filePath = currentPath / ".." / filename;
        if (fs::exists(filePath))
        {
            return filePath.string();
        }

        throw std::runtime_error("Could not find file: " + filename);
    }

    inline void loadEnv(const std::string &filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "Warning: Could not open .env file: " << filename << std::endl;
            return;
        }
        std::string line;
        while (std::getline(file, line))
        {
            size_t pos = line.find('=');
            if (pos != std::string::npos)
            {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                setenv(key.c_str(), value.c_str(), 1);
            }
        }
    }

} // namespace FileUtils