#pragma once

#include <memory>
#include <vector>
#include <array>
#include <optional>
#include <string>

#include "types.h"
#include "attributes.h"

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
    class Node
    {
    public:
        class State;

        Node();
        virtual ~Node() = default;

        void add(std::shared_ptr<Node>);
        void remove(std::shared_ptr<Node> const&);

        Node& set_force_intermediate(bool);
        bool force_intermediate() const;

        Node& set_visible(bool);
        bool visible() const;

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

        Node& set_flex(Flex);
        Flex const& flex() const;

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

    private:
        bool _layout_changed = true;

        bool _force_intermediate = false;

        //
        // intermediate rendering
        std::shared_ptr<Intermediate> _intermediate;

        //
        // sizing & visibility
        bool _visible;

        float _opacity;

        Sizing _width;
        Sizing _min_width;
        Sizing _max_width;
        Sizing _height;
        Sizing _min_height;
        Sizing _max_height;
        Sizing _left;
        Sizing _top;
        Sizing _bottom;
        Sizing _right;

        Flex _flex;

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
        void compute_content_properties(bool recursive=true);
    };

}