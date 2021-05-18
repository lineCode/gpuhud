#include <gtest/gtest.h>
#include <boost/spirit/home/x3.hpp>
// #include <boost/spirit/include/qi.hpp>

#include <gpugraph/css/grammar.h>

namespace x3 = boost::spirit::x3;

namespace gpugraph::css::grammar::tests
{

    TEST(Grammar, identifier_accepts_alpha_with_underscore_and_hyphen)
    {
        std::string input("a_b-c__d--");
        std::string parsed;
        
        auto iter     = input.cbegin();
        auto end_iter = input.cend();
        bool r = x3::phrase_parse(iter, end_iter, identifier, x3::ascii::space, parsed);
        
        ASSERT_TRUE(iter == end_iter);
        ASSERT_EQ(input, parsed);
    }

    TEST(Grammar, identifier_must_not_start_with_numeric)
    {
        std::string input("3abc");

        auto iter     = input.cbegin();
        auto end_iter = input.cend();
        bool r = x3::phrase_parse(iter, end_iter, identifier, x3::ascii::space);
    
        ASSERT_TRUE(iter != end_iter);
    }

    TEST(Grammar, length_unit_gets_parsed)
    {
        std::vector<std::tuple<float, Length::Unit>> expectation{
            {10.f, Length::Unit::Pixel},
            {1.2f, Length::Unit::FontSize }
        };
        std::vector<std::string> items{"10px", "1.2em"};
        std::ostringstream stream;
        std::copy(items.begin(), items.end(), std::ostream_iterator<std::string>(stream));
        auto input = stream.str();

        std::vector<std::tuple<float, Length::Unit>> result;

        auto iter     = input.cbegin();
        auto end_iter = input.cend();
        bool r = x3::phrase_parse(iter, end_iter, *length, x3::ascii::space, result);
    
        ASSERT_TRUE(iter == end_iter);
        ASSERT_EQ(result, expectation);
    }
}

