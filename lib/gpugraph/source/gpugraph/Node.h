#pragma once

#include <memory>
#include <vector>
#include <array>
#include <optional>
#include <string>

#include "types.h"
#include "Context.h"
#include "log.h"
#include "StyleHash.h"

namespace gpugraph
{

    class Intermediate;

    class StyleCollection;
    struct StyleBlock;
    using Styling = std::vector<std::shared_ptr<StyleBlock>>;

    class Node : public std::enable_shared_from_this<Node>
    {
    public:
        struct State;

        virtual ~Node() = default;
        static std::shared_ptr<Node> create(std::string type="div");

        //
        // tree
        Node const* parent() const;
        std::size_t size() const;
        
        void add(std::shared_ptr<Node>);
        void remove(std::shared_ptr<Node> const&);
        void clear();

        std::shared_ptr<Node> at(std::size_t);
        std::shared_ptr<Node> const& at(std::size_t) const;
        
        std::vector<std::shared_ptr<Node>>::const_iterator begin() const;
        std::vector<std::shared_ptr<Node>>::const_iterator end() const;

        std::vector<std::shared_ptr<Node>>::iterator begin();
        std::vector<std::shared_ptr<Node>>::iterator end();

        //
        // selector based styling
        Node& set_id(std::string);
        std::string const& id() const;
        std::string const& type() const;

        Node& add_class(std::string);
        Node& remove_class(std::string const&);
        std::set<std::string> const& class_set() const;

        //
        // set css style collection
        void set_style_collection(std::shared_ptr<StyleCollection>);

        void set_style(std::string);
        std::string const& style() const;

        //
        // used by the selector for optimizing the stylesheet linking,
        // contains classes with prefix ".", hash identifiers with "#" etc.
        StyleHash const& style_hash() const;

        //
        // rendering
        Node& set_force_intermediate(bool);
        bool force_intermediate() const;
        std::shared_ptr<Intermediate> const& intermediate() const;

        Node& set_text_content(std::string);
        std::string const& text_content() const;

        //
        // depth first traversal
        using StateVisitor = std::function<bool(Node&, Node::State&)>;
        void accept(StateVisitor);

    protected:
        Node(std::string type);

    private:
        struct StyleAlgorithm;
        // struct UpdatePass;
        // struct RenderPass;

        //
        // force an intermediate with this node as root
        bool _force_intermediate = false;

        //
        // intermediate rendering
        std::shared_ptr<Intermediate> _intermediate;

        //
        // styling
        std::shared_ptr<StyleCollection> _style_collection;
        StyleHash _style_hash;
        Styling _styling;
        std::shared_ptr<StyleBlock> _style;

        std::string _id;
        std::string _type;
        std::set<std::string> _class_set;

        //
        // tree structure
        Node* _parent = nullptr;
        std::vector<std::shared_ptr<Node>> _children;

        //
        // text ..
        std::string _text_content;

        //
        // (calculated) state, this is a 1:1-relationship
        std::shared_ptr<State> _state;
    };

    inline std::size_t Node::size() const
    {
        return _children.size();
    }

    inline std::vector<std::shared_ptr<Node>>::iterator Node::begin()
    {
        return _children.begin();
    }

    inline std::vector<std::shared_ptr<Node>>::iterator Node::end()
    {
        return _children.end();
    }

    inline std::vector<std::shared_ptr<Node>>::const_iterator Node::begin() const
    {
        return _children.begin();
    }

    inline std::vector<std::shared_ptr<Node>>::const_iterator Node::end() const
    {
        return _children.end();
    }

    inline std::shared_ptr<Node> Node::at(std::size_t index)
    {
        if (index >= size())
        {
            log_error("accessed \"" << this->_type << "\" with oob index");
            return nullptr;
        }
        return _children.at(index);
    }

    inline std::shared_ptr<Node> const& Node::at(std::size_t index) const
    {
        if (index >= size())
        {
            log_error("accessed \"" << this->_type << "\" with oob index");
            return nullptr;
        }
        return _children.at(index);
    }

}