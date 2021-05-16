#pragma once

#include <memory>
#include <vector>
#include <array>
#include <optional>
#include <string>

#include "types.h"
#include "attributes.h"
#include "Context.h"

namespace gpugraph
{

    class Intermediate;

    //
    // a node describes a thing in the world, which we
    // may can see or touch. it has no real size nor
    // colors. it's more than a descritption how it
    // should look and act and it's relationships.
    // The engine will then compute the real appearance
    // and save it to the `Node::State`.
    class Node : public std::enable_shared_from_this<Node>
    {
    public:
        class State;

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

        Node& set_font(std::shared_ptr<SkFont>);
        std::shared_ptr<SkFont> font() const;

        Node& set_display(Display);
        Display display() const;

        Node& set_visible(bool);
        bool visible() const;

        Node& set_position(Position);
        Position position() const;

        Node& set_width(Sizing);
        Sizing const& width() const;

        Node& set_max_width(Sizing);
        Sizing const& max_width() const;

        Node& set_min_width(Sizing);
        Sizing const& min_width() const;

        Node& set_max_height(Sizing);
        Sizing const& max_height() const;

        Node& set_min_height(Sizing);
        Sizing const& min_height() const;

        Node& set_height(Sizing);
        Sizing const& height() const;


        Node& set_left(Sizing);
        Sizing const& left() const;

        Node& set_top(Sizing);
        Sizing const& top() const;

        Node& set_bottom(Sizing);
        Sizing const& bottom() const;

        Node& set_right(Sizing);
        Sizing const& right() const;

        /*        Node& set_flex(Flex);
                Flex const& flex() const;*/

        Node& set_opacity(float);
        float opacity() const;

        Node& set_padding(vec4);
        vec4 const& padding() const;

        Node& set_border_width(vec4);
        vec4 border_width() const;

        Node& set_border_radius(vec4);
        vec4 border_radius() const;

        Node& set_spacing(vec4);
        vec4 const& spacing() const;

        Node& set_border_color(color);
        color const& border_color() const;

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

    protected:
        Node(std::string type);

    private:
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
        Display _display; // e. g. the type of node
        Position _position = Position::Static;
        bool _visible = true;
        real_t _opacity = 1.f;
        std::shared_ptr<SkFont> _font;

        Sizing _width = SizingKeyword::Unset;
        Sizing _min_width = SizingKeyword::Unset;
        Sizing _max_width = SizingKeyword::Unset;
        Sizing _height = SizingKeyword::Unset;
        Sizing _min_height = SizingKeyword::Unset;
        Sizing _max_height = SizingKeyword::Unset;
        Sizing _left = SizingKeyword::Unset;
        Sizing _top = SizingKeyword::Unset;
        Sizing _bottom = SizingKeyword::Unset;
        Sizing _right = SizingKeyword::Unset;

        vec4 _margin;
        vec4 _padding;

        vec4 _border_width;

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