#include "../include/ConfigReader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

ConfigReader::ConfigReader(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open config files: " + filename);
    }

    std::string line, section;
    while (std::getline(file, line))
    {
        // istringsstream is used to parse the line
        std::istringstream is_line(line);
        std::string key;
        if (line[0] == '[' && line[line.size() - 1] == ']')
        {
            section = line.substr(1, line.size() - 2);
        }
        else if (std::getline(is_line, key, '='))
        {
            std::string value;
            if (std::getline(is_line, value))
            {
                data[section][key] = value;
            }
        }
    }
}

std::string ConfigReader::get(const std::string &section, const std::string &key) const
{
    std::string envKey = section + "_" + key;
    std::string envValue = getEnv(envKey);
    if (!envValue.empty())
    {
        return envValue;
    }

    auto s = data.find(section);
    if (s != data.end())
    {
        auto k = s->second.find(key);
        if (k != s->second.end())
        {
            return k->second;
        }
    }
    throw std::runtime_error("Config key not found: " + section + "." + key);
}

std::string ConfigReader::getEnv(const std::string &key, const std::string &defaultValue)
{
    const char *value = std::getenv(key.c_str());
    return value ? value : defaultValue;
}