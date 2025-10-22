#pragma once
#include <string>
#include <map>
#include <INIReader.h>

namespace IniUtils {
    using IniData = std::map<std::string, std::map<std::string, std::string>>;

    IniData LoadIniFromString(const std::string& content);

    std::vector<float> ParseFloatArray(const std::string& value);

    std::vector<float> ParseFloatArrayBlock(const std::string &block);

    std::string RemoveComments(const std::string &src);

    void Trim(std::string &s);
}
