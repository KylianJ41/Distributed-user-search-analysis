#include "../include/ConfigReader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

ConfigReader::ConfigReader(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open config file: " + filename);
    }

    std::string line;
    std::string section;
    while (std::getline(file, line))
    {
        if (line[0] == '[' && line[line.size() - 1] == ']')
        {
            section = line.substr(1, line.size() - 2);
        }
        else
        {
            auto pos = line.find('=');
            if (pos != std::string::npos)
            {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                data[section][key] = value;
            }
        }
    }
}

std::string ConfigReader::get(const std::string &section, const std::string &key) const
{
    auto s = data.find(section);
    if (s != data.end())
    {
        auto k = s->second.find(key);
        if (k != s->second.end())
        {
            std::string value = k->second;
            // Check if the value is an environment variable
            if (value.size() > 3 && value[0] == '$' && value[1] == '{' && value[value.size() - 1] == '}')
            {
                std::string envVar = value.substr(2, value.size() - 3);
                const char *envValue = std::getenv(envVar.c_str());
                if (envValue)
                {
                    return envValue;
                }
            }
            return value;
        }
    }
    throw std::runtime_error("Config key not found: " + section + "." + key);
}

std::string ConfigReader::getEnv(const std::string &key, const std::string &defaultValue)
{
    const char *value = std::getenv(key.c_str());
    return value ? value : defaultValue;
}