#pragma once

#include <string>

namespace gpugraph
{

    class Selector
    {
    public:
        static Selector parse(std::string const&);
    };

}