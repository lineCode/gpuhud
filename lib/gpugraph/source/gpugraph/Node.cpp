#include "Node.h"
#include "NodeState.h"
#include "Intermediate.h"

namespace gpugraph
{

    Node::Node(std::string type)
        : _type(std::move(type))
        , _state(std::make_shared<State>())
    {
    }

    std::shared_ptr<Node> Node::create(std::string type)
    {
        return std::shared_ptr<Node>(new Node(std::move(type)));
    }

    void Node::add(std::shared_ptr<Node> node)
    {
        _children.push_back(node);
        node->_parent = this;
        node->compute_content_properties(true);
    }

    void Node::remove(std::shared_ptr<Node> const& node)
    {
        _children.erase(std::remove_if(_children.begin(), _children.end(), [&](auto& child) {
            return node == child;
        }), _children.end());
    }

    Node& Node::set_id(std::string value)
    {
        _id = std::move(value);
        return *this;
    }

    std::string const& Node::id() const
    {
        return _id;
    }

    Node& Node::add_class(std::string class_)
    {
        _class_set.insert(std::move(class_));
        return *this;
    }

    Node& Node::remove_class(std::string const& class_)
    {
        _class_set.erase(class_);
        return *this;
    }

    std::set<std::string> const& Node::class_set() const
    {
        return _class_set;
    }

    Node& Node::set_force_intermediate(bool value)
    {
        _force_intermediate = value;
        if (!_intermediate)
            _intermediate = std::make_shared<Intermediate>();
        return *this;
    }

    bool Node::force_intermediate() const
    {
        return _force_intermediate;
    }

    Node& Node::set_text_content(std::string value)
    {
        _text_content = std::move(value);
        return *this;
    }

    std::string const& Node::text_content() const
    {
        return _text_content;
    }

    std::shared_ptr<Intermediate> const& Node::intermediate() const
    {
        return _intermediate;
    }

    void Node::accept(StateVisitor visitor)
    {
        if (visitor(*this, *_state))
            for (auto& child : _children)
                accept(visitor);
    }

    void Node::compute_content_properties(bool recursive)
    {
        if (recursive)
        {
            for (auto& child : _children)
                child->compute_content_properties(true);
        }
        //
        // throughts/interpretation (css):
        // normally, text as content is a one-liner. min-content & max-content & content of 
        // text is the same. if width is set to any "content"-property, it'll become a 
        // one-liner. if width is set with px or .. it depends on "word-wrap".
        // For this, we may use the Skia-Paragraph-Builder..?
        //
        // consequence: 
        //   we need to compute the text width (and height) here of any text-content.
        //   setting min-width etc. explicitely will override the computed one.

        //
        // todo: set root font, calc rem
    }

}

