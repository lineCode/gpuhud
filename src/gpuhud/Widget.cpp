#include "Widget.h"

namespace gpuhud
{

    Widget::Widget()
    {
    }

    std::shared_ptr<gpugraph::Node> const& Widget::root_node()
    {
        return _root_node;
    }

    void Widget::set_root_node(std::shared_ptr<gpugraph::Node> node)
    {
        _root_node = std::move(node);
    }

}