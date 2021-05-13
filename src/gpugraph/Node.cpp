#include "Node.h"

namespace gpugraph
{

    Node::Node()
    {
    }

    void Node::add(std::shared_ptr<Node> node)
    {
        _children.push_back(std::move(node));
    }

    void Node::remove(std::shared_ptr<Node> const& node)
    {
        _children.erase(std::remove_if(_children.begin(), _children.end(), [&](auto& child) {
            return node == child;
        }), _children.end());
    }

    Node& Node::set_use_intermediate(bool value)
    {
        _use_intermediate = value;
        return *this;
    }

    bool Node::use_intermediate() const
    {
        return _use_intermediate;
    }

    Node& Node::set_visible(bool value)
    {
        _visible = value;
        return *this;
    }

    bool Node::visible() const
    {
        return _visible;
    }

    Node& Node::set_width(Sizing value)
    {
        _width = std::move(value);
        return *this;
    }

    Sizing const& Node::width() const
    {
        return _width;
    }

    Node& Node::set_height(Sizing value)
    {
        _height = std::move(value);
        return *this;
    }

    Sizing const& Node::height() const
    {
        return _height;
    }

    Node& Node::set_flex(Flex value)
    {
        _flex = std::move(value);
        return *this;
    }

    Flex const& Node::flex() const
    {
        return _flex;
    }

    Node& Node::set_opacity(float value)
    {
        _opacity = value;
        return *this;
    }

    float Node::opacity() const
    {
        return _opacity;
    }

}
