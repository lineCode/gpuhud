#pragma once

#include <istream>
#include <memory>

namespace gpugraph
{
    
    class Node;

    class Loader
    {
    public:
        static std::shared_ptr<Node> load(std::istream&);

    private:
    };

}