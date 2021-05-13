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

    private:
        std::shared_ptr<gpugraph::Node> _root_node;
        std::shared_ptr<gpugraph::Node> _content_node;
        std::vector<std::shared_ptr<Widget>> _children;
    };

}