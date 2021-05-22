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
#include <gtest/gtest.h>

#include <gpugraph/string_utils.h>

TEST(StringUtils, test_tokenize)
{
    std::string input = R"~(.a .bc #id)~";
    auto result = gpugraph::tokenize(input, "\\s+");
    ASSERT_EQ(result.size(), 3);
    ASSERT_EQ(result.at(0), ".a");
    ASSERT_EQ(result.at(1), ".bc");
    ASSERT_EQ(result.at(2), "#id");
}

TEST(StringUtils, test_if_string_without_delimiter_gets_returned)
{
    std::string input = R"~( aaa bbb  cccc  )~";
    auto result = gpugraph::tokenize(input, "\\s+");
    ASSERT_EQ(result.size(), 3);
}

TEST(StringUtils, test_tokrnize_css_selector1)
{
    /*
    std::string input = R"~(.c.as.qwe#qwe:hover)~";
    auto result = gpugraph::tokenize(input, R"~("[\.#:][^\.#:]+")~", -1);
    std::vector<std::string> x{".c", ".as", ".qwe", "#qwe", ":hover"};
    ASSERT_EQ(result, x);*/
}

