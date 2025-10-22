#include "Util/IniUtils.h"

#include <ini.h>
#include <sstream>
#include <iostream>

namespace IniUtils {

    static int IniHandler(void* user, const char* section, const char* name, const char* value) {
        auto* data = static_cast<IniData*>(user);
        (*data)[section][name] = value ? value : "";
        return 1; // continue parsing
    }

    IniData LoadIniFromString(const std::string& content) {
        IniData result;
        if (ini_parse_string(content.c_str(), IniHandler, &result) < 0) {
            std::cerr << "[IniUtils] Failed to parse INI from string!" << std::endl;
        }
        return result;
    }

    std::vector<float> ParseFloatArray(const std::string& value) {
        std::stringstream ss(value);
        std::vector<float> arr;
        float f;
        while (ss >> f) {
            arr.push_back(f);
        }
        return arr;
    }

    std::vector<float> ParseFloatArrayBlock(const std::string &block) {
        std::stringstream ss(block);
        std::string line;
        std::vector<float> result;

        while (std::getline(ss, line)) {
            auto pos = line.find('#');
            if (pos != std::string::npos)
                line = line.substr(0, pos);

            line.erase(line.begin(),
                       std::find_if(line.begin(), line.end(), [](unsigned char c) { return !std::isspace(c); }));
            line.erase(std::find_if(line.rbegin(), line.rend(), [](unsigned char c) { return !std::isspace(c); }).base(),
                       line.end());

            if (line.empty())
                continue;

            std::stringstream ls(line);
            float f;
            while (ls >> f)
                result.push_back(f);
        }

        return result;
    }

    std::string CollectMultilineBlock(std::stringstream& ss) {
        std::string block, line;
        while (std::getline(ss, line)) {
            // Trim
            line.erase(line.begin(), std::find_if(line.begin(), line.end(),
                                                  [](unsigned char c){ return !std::isspace(c); }));
            line.erase(std::find_if(line.rbegin(), line.rend(),
                                    [](unsigned char c){ return !std::isspace(c); }).base(), line.end());

            if (line.empty()) continue;
            if (line[0] == '[') {
                ss.seekg(-static_cast<int>(line.size())-1, std::ios_base::cur); // откатываемся
                break;
            }

            block += line + " ";
        }
        return block;
    }

    std::string RemoveComments(const std::string &src) {
        std::stringstream input(src);
        std::stringstream output;
        std::string line;
        while (std::getline(input, line)) {
            auto pos = line.find('#');
            if (pos != std::string::npos)
                line = line.substr(0, pos);
            line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
            if (!line.empty())
                output << line << " ";
        }
        return output.str();
    }

    void Trim(std::string &s) {
        s.erase(std::remove(s.begin(), s.end(), '\r'), s.end());
        while (!s.empty() && std::isspace((unsigned char)s.back()))
            s.pop_back();
        while (!s.empty() && std::isspace((unsigned char)s.front()))
            s.erase(s.begin());
    }

} // namespace IniUtils
