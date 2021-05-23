/***************************************************************************//*/
  Copyright (c) 2021 Martin Rudoff

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software. 

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
/******************************************************************************/

#ifndef __LOG_
#define __LOG_

#include <cstdint>
#include <iostream>
#include <filesystem>
#include <algorithm>

#include <rang.hpp>

#ifndef LOG_LEVEL
#  define LOG_LEVEL 100
#endif

constexpr std::uint32_t __log_level = LOG_LEVEL;

constexpr std::uint32_t __log_level_none = 0;
constexpr std::uint32_t __log_level_fatal = 1;
constexpr std::uint32_t __log_level_error = 2;
constexpr std::uint32_t __log_level_warn = 3;
constexpr std::uint32_t __log_level_info = 4;
constexpr std::uint32_t __log_level_debug = 5;

namespace
{

    inline std::string __log_format_file(std::string file)
    {    
        std::transform(file.begin(), file.end(), file.begin(), [](auto c) { return std::tolower(c); });
        std::filesystem::path path(file);
        return path.stem().string() + path.extension().string();
    }

    struct __log_start
    {
    } log_start;

    struct __log_reset
    {
    } log_reset;

    std::ostream& operator<<(std::ostream& stream, __log_start const&)
    {
        stream << rang::style::bold << rang::fg::yellow << rang::bg::black;
        return stream;
    }

    std::ostream& operator<<(std::ostream& stream, __log_reset const&)
    {
        stream << rang::fg::reset << rang::bg::reset << rang::style::reset;
        return stream;
    }

}

#define log_fatal(text) if constexpr(__log_level >= __log_level_fatal) \
    std::cout \
    << log_start << rang::fg::red \
    << "[fat!]" \
    << " " << text \
    << "  (" << __log_format_file(__FILE__) << ":" << __LINE__ << ")" \
    << log_reset \
    << std::endl; \
    exit(-1);

#define log_error(text) if constexpr(__log_level >= __log_level_error) \
    std::cout \
    << log_start << rang::fg::red << rang::style::rblink \
    << "[err!]" \
    << " " << text \
    << "  (" << __log_format_file(__FILE__) << ":" << __LINE__ << ")" \
    << log_reset \
    << std::endl;

#define log_warn(text) if constexpr(__log_level >= __log_level_warn) \
    std::cout \
    << log_start << rang::fg::yellow \
    << "[warn]" \
    << " " << text \
    << log_start << rang::fg::yellow \
    << "  (" << __log_format_file(__FILE__) << ":" << __LINE__ << ")" \
    << log_reset \
    << std::endl;

#define log_info(text) if constexpr(__log_level >= __log_level_info) \
    std::cout \
    << log_start << rang::fg::gray \
    << "[info]" \
    << log_reset \
    << " " << text \
    << log_start << rang::fg::gray \
    << "  (" << __log_format_file(__FILE__) << ":" << __LINE__ << ")" \
    << log_reset \
    << std::endl;

#define log_debug(text) if constexpr(__log_level >= __log_level_debug) \
    std::cout \
    << log_start << rang::fg::gray \
    << "[dbug]" \
    << log_reset \
    << " " << text \
    << log_start << rang::fg::gray \
    << "  (" << __log_format_file(__FILE__) << ":" << __LINE__ << ")" \
    << log_reset \
    << std::endl;

#define log_with_level(level, text) if constexpr(__log_level >= level) \
    std::cout \
    << log_start << rang::fg::gray \
    << "[l" << std::setw(3) << std::setfill('0') << level << "]" \
    << log_reset \
    << " " << text \
    << log_start << rang::fg::gray \
    << "  (" << __log_format_file(__FILE__) << ":" << __LINE__ << ")" \
    << log_reset \
    << std::endl;

#endif
