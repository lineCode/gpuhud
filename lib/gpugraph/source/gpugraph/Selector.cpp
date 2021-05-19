#include "Selector.h"

#include "string_utils.h"

namespace gpugraph
{

    /*
    ("old") CSS 2.1. "simple selector" is important.
    hash, class, name are simple rules. 

    attrib
      : '[' S* IDENT S* [ [ '=' | INCLUDES | DASHMATCH ] S* [ IDENT | STRING ] S* ]? ']'
    ruleset
      : selector [ ',' S* selector ]* '{' S* declaration? [ ';' S* declaration? ]* '}' S*
    combinator
      : '+' S*
      | '>' S*
    pseudo
      : ':' [ IDENT | FUNCTION S* [IDENT S*]? ')' ]
    selector
      : simple_selector [ combinator selector | S+ [ combinator? selector ]? ]?
    simple_selector
      : element_name [ HASH | class | attrib | pseudo ]* | [ HASH | class | attrib | pseudo ]+
    */
    Selector Selector::parse(std::string const& expression)
    {
        static auto espace = std::regex(R"~(\\s+)~");
        std::sregex_token_iterator it{ expression.begin(), expression.end(), espace, -1}, last;
        for (; it != std::sregex_token_iterator(); ++it)
        {   
            std::string token = it->str();
            if (token.empty())
                continue;
            if (token == ">")
            {
            }
            else
            {
                auto result = gpugraph::tokenize(token, "[\.#:][^\.#:]+");
            }
        }
        throw "";
    }

}