#pragma once
#include <string>
#include <map>

class ConfigReader
{
public:
    ConfigReader(const std::string &filename);
    std::string get(const std::string &section, const std::string &key) const;
    static std::string getEnv(const std::string &key, const std::string &defaultValue = "");

private:
    std::map<std::string, std::map<std::string, std::string>> data;
};