#include "ChildWindow.h"

namespace gpuhud
{

    ChildWindow::ChildWindow()
        : _header_row(gpugraph::Node::create())
        , _title_label(gpugraph::Node::create())
        , _close_button(gpugraph::Node::create())
    {
        using namespace gpugraph;
        root_node()->set_display(Display::Flex);
        root_node()->set_min_width(10 | Length::px);
        root_node()->set_min_height(10 | Length::px);
        root_node()->add(_header_row);

        _header_row->add(_title_label);
        _header_row->add(_close_button);

        auto content_node = gpugraph::Node::create();
        root_node()->add(content_node);
        set_content_node(content_node);
    }

}
