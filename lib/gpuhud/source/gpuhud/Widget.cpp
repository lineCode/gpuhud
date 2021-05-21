#include "Widget.h"

namespace gpuhud
{

    Widget::Widget(std::shared_ptr<gpugraph::Node> root_node)
        : _root_node(root_node ? root_node : gpugraph::Node::create())
    {
    }

    std::shared_ptr<gpugraph::Node> const& Widget::root_node()
    {
        return _root_node;
    }

}
