#pragma once

#include <istream>
#include <memory>

namespace gpugraph
{
    
    class Node;

    class XmlLoader
    {
    public:
        static std::shared_ptr<Node> load(std::istream&);
        static std::shared_ptr<Node> load(std::string const&);
    private:
    };

}
