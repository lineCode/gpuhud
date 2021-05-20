#pragma once

#include <memory>
#include <vector>
#include <array>
#include <optional>
#include <string>

#include "types.h"
#include "Context.h"

namespace gpugraph
{

    class Style;
    class Intermediate;

    class Node : public std::enable_shared_from_this<Node>
    {
    public:
        struct State;

        virtual ~Node() = default;
        static std::shared_ptr<Node> create(std::string type="div");

        void add(std::shared_ptr<Node>);
        void remove(std::shared_ptr<Node> const&);

        Node& set_id(std::string);
        std::string const& id() const;

        Node& add_class(std::string);
        Node& remove_class(std::string const&);
        std::set<std::string> const& class_set() const;

        Node& set_force_intermediate(bool);
        bool force_intermediate() const;

        Node& set_text_content(std::string);
        std::string const& text_content() const;

        std::shared_ptr<Intermediate> const& intermediate() const;

        using StateVisitor = std::function<bool(Node&, Node::State&)>;
        void accept(StateVisitor);

        std::size_t size() const;
        
        std::shared_ptr<Node> at(std::size_t);
        std::shared_ptr<Node> const& at(std::size_t) const;
        
        std::vector<std::shared_ptr<Node>>::const_iterator begin() const;
        std::vector<std::shared_ptr<Node>>::const_iterator end() const;

        std::vector<std::shared_ptr<Node>>::iterator begin();
        std::vector<std::shared_ptr<Node>>::iterator end();

        void set_style(std::shared_ptr<Style>);

    protected:
        Node(std::string type);

    private:
        std::shared_ptr<Style> _style;

        bool _layout_changed = true;
        bool _force_intermediate = false;
        //
        // intermediate rendering
        std::shared_ptr<Intermediate> _intermediate;

        std::string _type;
        std::string _id;
        std::set<std::string> _class_set;

        //
        // sizing & visibility
        bool _visible = true;

        struct
        {
            bool active= false;
            bool hovered= false;
            bool focused = false;
        } _input;

        real_t _opacity = 1.f;

        std::string _text_content;

        //
        // tree structure
        Node* _parent;
        std::vector<std::shared_ptr<Node>> _children;

        //
        // (calculated) state
        std::shared_ptr<State> _state;

    private:

        //
        // algorithms
        void compute_content_properties(bool recursive);
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
        return _children.at(index);
    }

    inline std::shared_ptr<Node> const& Node::at(std::size_t index) const
    {
        return _children.at(index);
    }


}