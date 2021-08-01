#include <string>
#include <vector>

#ifndef STRING_HEADER
#define STRING_HEADER

class String {
public:
    inline static bool startsWith(std::string string, std::string subString) {
        if (string.substr(0, subString.size()) == subString) return true;
        else return false;
    }

    inline static std::vector<std::string> split(std::string string, std::string match) {
        std::vector<std::string> result;
        int matchSize = match.size();
        int lastIndex = 0;
        for (int index = 0; index < string.size(); index++) {
            if (index == string.size() - match.size() + 1) {
                result.push_back(string.substr(lastIndex, string.size() - lastIndex));
                break;
            }
            if (string.substr(index, matchSize) == match) {
                result.push_back(string.substr(lastIndex, index - lastIndex));
                lastIndex = matchSize + index;
            }
        }
        return result;
    }

    inline static std::string replace(std::string string, std::string match, std::string replace) {
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
};


#endif