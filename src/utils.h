#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>

inline std::string toUpper(std::string s) {
    for (char& c : s) c = toupper(c);
    return s;
}

inline std::vector<std::string> tokenize(const std::string& line) {
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string tok;
    while (iss >> tok) tokens.push_back(tok);
    return tokens;
}

inline std::string removeComment(const std::string& line) {
    size_t pos = line.find(';');
    return (pos != std::string::npos) ? line.substr(0, pos) : line;
}

inline std::string normalizeSpaces(const std::string& line) {
    std::string result;
    bool lastWasSpace = false;
    for (char c : line) {
        if (c == '\t' || c == ' ') {
            if (!lastWasSpace && !result.empty()) {
                result += ' ';
                lastWasSpace = true;
            }
        } else {
            result += c;
            lastWasSpace = false;
        }
    }
    if (!result.empty() && result.back() == ' ')
        result.pop_back();
    return result;
}

inline std::string stripColon(const std::string& s) {
    if (!s.empty() && s.back() == ':')
        return s.substr(0, s.size() - 1);
    return s;
}

inline int parseValue(const std::string& s) {
    if (s.size() > 2 && s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
        return std::stoi(s, nullptr, 16);
    return std::stoi(s);
}