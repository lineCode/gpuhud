#include "Widget.h"

namespace gpuhud
{

    Widget::Widget()
        :_root_node(gpugraph::Node::create())
    {
    }

    std::shared_ptr<gpugraph::Node> const& Widget::root_node()
    {
        return _root_node;
    }

}