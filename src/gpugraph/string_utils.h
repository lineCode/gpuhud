#pragma once

#include <string>
#include <regex>

namespace gpugraph
{

    inline void ltrim(std::string& s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
    }

    inline void rtrim(std::string& s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    inline void trim(std::string& s) {
        ltrim(s);
        rtrim(s);
    }

    inline std::string ltrimmed(std::string const& s) {
        auto r = s;
        ltrim(r);
        return r;
    }

    inline std::string rtrimmed(std::string const& s) {
        auto r = s;
        rtrim(r);
        return r;
    }

    inline std::string trimmed(std::string s) {
        auto r = s;
        trim(r);
        return r;
    }

    inline std::vector<std::string> tokenize(std::string const& input, std::string const& regex, int sup=-1)
    {
        std::regex re(regex);
        std::sregex_token_iterator rit{ input.begin(), input.end(), re, sup }, last;
        std::vector<std::string> tokens;
        std::remove_copy_if(rit, std::sregex_token_iterator(), std::back_inserter(tokens),
                    [](std::string const &s) { return s.empty(); });
        return tokens;
    }

    inline bool ends_with(std::string const& str, std::string const& ending)
    {
        if (str.length() >= ending.length()) {
            return (0 == str.compare(str.length() - ending.length(), ending.length(), ending));
        }
        else {
            return false;
        }
    }

}
