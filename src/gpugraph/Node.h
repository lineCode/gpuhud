#pragma once

#include <memory>
#include <vector>
#include <array>

#include "types.h"
#include "attributes.h"

namespace gpugraph
{

    class Intermediate;

    class Node
    {
    public:
        Node();

        void add(std::shared_ptr<Node>);
        void remove(std::shared_ptr<Node> const&);

        Node& set_force_intermediate(bool);
        bool force_intermediate() const;

        Node& set_visible(bool);
        bool visible() const;

        Node& set_width(Sizing);
        Sizing const& width() const;

        Node& set_height(Sizing);
        Sizing const& height() const;

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

        std::shared_ptr<Intermediate> const& intermediate() const;

    private:
        bool _layout_changed = true;
        
        bool _force_intermediate = false;
        std::shared_ptr<Intermediate> _intermediate;

        bool _visible;
        float _opacity;
        
        Sizing _width;
        Sizing _height;
        Flex _flex;

        vec4 _margin;
        vec4 _border_width;
        vec4 _padding;

        Node* _parent;
        std::vector<std::shared_ptr<Node>> _children;

        std::array<vec2, 4> _vertices;
    };

}