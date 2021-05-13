#pragma once

#include <memory>
#include <vector>

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

        void set_use_intermediate(bool);
        bool use_intermediate() const;

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

    private:
        bool _layout_changed = true;
        
        bool _use_intermediate = false;
        std::shared_ptr<Intermediate> _intermediate;

        Flex _flex;

        vec4 _margin;
        vec4 _border_width;
        vec4 _padding;

        Node* _parent;
        std::vector<std::shared_ptr<Node>> _children;
    };

}