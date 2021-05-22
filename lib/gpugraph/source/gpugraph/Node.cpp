/***************************************************************************//*/

  Copyright (c) 2021 Martin Rudoff

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software. 

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

/******************************************************************************/

#include "Node.h"
#include "NodeState.h"
#include "Intermediate.h"
#include "StyleCollection.h"
#include "StyleCompiler.h"
#include "NodeStyleAlgorithm.h"

namespace gpugraph
{

    Node::Node(std::string type)
        : _type(type)
        , _style_collection(std::make_shared<StyleCollection>())
    {
        _style_hash.insert_type(_type);
        define_attribute("class", [this]() {
            return "";
        }, [this](std::optional<std::string> value) {
            // ...
        });
    }

    void Node::on_after_dynamic_attribute_changed()
    {
    }

    void Node::rebuilder_style_hash()
    {
    }
            
    std::string const& Node::type() const
    {
        return _type;
    }

    std::shared_ptr<Node> Node::create(std::string type)
    {
        return std::shared_ptr<Node>(new Node(std::move(type)));
    }

    void Node::add(std::shared_ptr<Node> node)
    {
        _children.push_back(node);
        node->_parent = this;
        node->_style_collection = _style_collection;
        StyleAlgorithm().link_style_recursively(*node);
    }

    void Node::remove(std::shared_ptr<Node> const& node)
    {
        _children.erase(std::remove_if(_children.begin(), _children.end(), [&](auto& child) {
            return node == child;
        }), _children.end());
        // TODO:
        // recompute content ..
    }

    void Node::clear()
    {
        _children.clear();
    }

    Node& Node::add_class(std::string class_)
    {
        _style_hash.insert_class(class_);
        _class_set.insert(std::move(class_));
        StyleAlgorithm().link_style_recursively(*this);
        return *this;
    }

    Node& Node::remove_class(std::string const& class_)
    {
        _class_set.erase("." + class_);
        StyleAlgorithm().link_style_recursively(*this);
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
        // TODO
        // recompute content
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
        if (visitor(*this))
            for (auto& child : _children)
                accept(visitor);
    }

    void Node::set_style_collection(std::shared_ptr<StyleCollection> style)
    {
        if (!style)
            return;
        _style_collection = std::move(style);
        StyleAlgorithm().link_style_recursively(*this);
    }

    StyleHash const& Node::style_hash() const
    {
        return _style_hash;
    }

    Node const* Node::parent() const
    {
        return _parent;
    }

    void Node::set_style(std::string source) 
    {
        StyleCompiler([this](auto block) {
            _style = std::move(block);
        }).compile(source);
    }

}
