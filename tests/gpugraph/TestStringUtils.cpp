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
