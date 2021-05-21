#ifndef __LOG_
#define __LOG_

//
// use evil macros to be able to disable logging evaluation.
// we can log like: log_warn("something went wrong:" << what)
//

#include <iostream>
#include <filesystem>
#include <algorithm>

#ifndef LOG_LEVEL
#  define LOG_LEVEL 100
#endif

constexpr unsigned int __log_level = LOG_LEVEL;

constexpr unsigned int __log_level_none = 0;
constexpr unsigned int __log_level_error = 1;
constexpr unsigned int __log_level_warn = 2;
constexpr unsigned int __log_level_info = 3;
constexpr unsigned int __log_level_debug = 4;

inline std::string __log_format_file(std::string file)
{    
    std::transform(file.begin(), file.end(), file.begin(), [](auto c) { return std::tolower(c); });
    std::filesystem::path path(file);
    return path.stem().string() + path.extension().string();
}

#define log_fatal(text) std::cerr << "[debug:" << __log_format_file(__FILE__) << ":" << __LINE__ << "]" << text; exit(-1)
#define log_error(text) if constexpr(__log_level >= __log_level_warn) std::cerr << "[error:" << __log_format_file(__FILE__) << ":" << __LINE__ << "] " << text << std::endl;
#define log_warn(text) if constexpr(__log_level >= __log_level_warn) std::cout << "[warn:" << __log_format_file(__FILE__) << ":" << __LINE__ << "] " << text << std::endl;
#define log_info(text) if constexpr(__log_level >= __log_level_warn) std::cout << "[info:" << __log_format_file(__FILE__) << ":" << __LINE__ << "] " << text << std::endl;
#define log_debug(text) if constexpr(__log_level >= __log_level_warn) std::cout << "[debug:" << __log_format_file(__FILE__) << ":" << __LINE__ << "] " << text << std::endl;
#define log_with_level(level, text) if constexpr(__log_level >= level) std::cout << "[lvl" << level << ":" << __log_format_file(__FILE__) << ":" << __LINE__ << "] " << text << std::endl;

#endif
