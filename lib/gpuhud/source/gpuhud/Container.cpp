#include "Container.h"

namespace gpuhud
{
    Container::Container()
        : Widget()
        , _content_node(root_node())
    {
    }

    void Container::add(std::shared_ptr<Widget> widget)
    {
        _content_node->add(widget->root_node());
        _children.push_back(widget);
    }

    void Container::remove(std::shared_ptr<Widget> widget)
    {
        _children.erase(std::remove_if(_children.begin(), _children.end(), [&](auto& child) {
            if (child == widget)
            {
                _content_node->remove(child->root_node());
                return true;
            }
            return false;
        }), _children.end());
    }

    void Container::clear()
    {
        for (auto& widget : _children)
            _content_node->remove(widget->root_node());
        _children.clear();
    }

    void Container::set_content_node(std::shared_ptr<gpugraph::Node> node)
    {
        _content_node = node;
    }

}
