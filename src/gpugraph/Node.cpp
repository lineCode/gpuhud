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

}
