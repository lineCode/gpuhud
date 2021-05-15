#pragma once

#include <memory>
#include <vector>

#include <gpugraph/Node.h>

namespace gpuhud
{

    class Widget
    {
    public:
        Widget();

        std::shared_ptr<gpugraph::Node> const& root_node();
    
    private:
        std::shared_ptr<gpugraph::Node> _root_node;
    };

}