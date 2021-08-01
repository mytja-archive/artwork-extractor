#include <string>
#include <vector>
#include <algorithm>

#ifndef STRING_HEADER
#define STRING_HEADER

class Strings {
public:
    static inline bool startsWith(std::string string, std::string subString) {
        if (string.substr(0, subString.size()) == subString) return true;
        else return false;
    }

    static inline std::vector<std::string> split(std::string string, std::string match) {
        std::vector<std::string> result = {};
        int matchSize = match.size();
        int last = 0;
        for (int index = 0; index < string.size(); index++) {
            if (string.substr(index, matchSize) == match) {
                result.emplace_back(string.substr(last, index - last));
                last = matchSize + index;
            }
        }
        result.emplace_back(string.substr(last, string.size()));
        return result;
    }

    static inline std::string replace(std::string string, std::string match, std::string replace) {
        std::string result;
        int matchSize = match.size();
        int lastIndex = 0;
        int matchCounter = 0;
        for (int index = 0; index < string.size(); index++) {
            if (matchCounter != 0) {
                if (matchCounter == match.size() - 1) matchCounter = 0;
                else matchCounter++;
            }
            else if (index == string.size() - match.size()) {
                result.append(replace);
                break;
            }
            else if (string.substr(index, matchSize) == match && matchCounter == 0) {
                result.append(replace);
                lastIndex = matchSize + index;
                matchCounter++;
            }
            else {
                result.push_back(string[index]);
            }
        }
        return result;
    }

    static inline std::string toUpperCase(std::string& string) {
        std::transform(string.begin(), string.end(), string.begin(), ::toupper);
        return string;
    }
};


#endif