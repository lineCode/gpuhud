#pragma once

#include "types.h"
#include "Rectangle.h"
#include "Context.h" // for skia
#include <css/parser.hpp>
#include <css/color.hpp>

namespace gpugraph
{
    using color = css::color;

    using Bounds = Rectangle<float>;

    struct NodeState
    {
        //
        // hm ..
        bool visible;

        //
        // hierachical
        css::display display;
        css::position position; // <- defaulted?
        
        //
        // interaction
        bool active= false;
        bool hovered= false;
        bool focused = false;

        //
        // computed
        // real_t width = 0.f;
        // real_t height = 0.f;
        Bounds border_box;
        Bounds content_box;
        real_t width_min_content; // [px]
        real_t width_max_content; // [px]
        real_t height_min_content; // [px]
        real_t height_max_content; // [px]

        bool layout_dirty = true;
        bool needs_redraw = true;

        struct Inherited
        {

            gpugraph::color color;
            std::shared_ptr<SkFont> font;
        
        } inherited;

        struct Defaulted
        {

            enum Edge
            {
                Left = 0,
                Top = 1,
                Right = 2,
                Bottom = 3
            };

            enum Corner
            {
                TopLeft = 0,
                TopRight = 1,
                BottomRight = 2,
                BottomLeft = 3
            };
            real_t _opacity = 1.f;

            std::array<std::optional<real_t>, 4> distance = std::array<std::optional<real_t>, 4>();
            std::array<real_t, 4> border_width = {0.f, 0.f, 0.f, 0.f};
            std::array<real_t, 4> border_radius = {0.f, 0.f, 0.f, 0.f};
            std::array<real_t, 4> margin = {0.f, 0.f, 0.f, 0.f};
            std::array<real_t, 4> padding = {0.f, 0.f, 0.f, 0.f};

        } defaulted;

        //
        // used for resetting the state
        static Defaulted defaults;

        void inherit(NodeState&);
        void reset();
    };

}


/* (default) inheritance of properties (~ :-) ):
            DONE
            font-weight
            font-family
            font-size
            font-style
            font-variant
            font
            color
            
            IMPORTANT
            direction
            cursor
            white-space
            word-break
            word-spacing
            letter-spacing
            word-wrap
            text-align
            text-align-last
            text-decoration-color
            text-indent
            text-justify
            text-shadow
            text-transform

            NICE T. HAVE
            border-collapse
            border-spacing
            caption-side
            font-size-adjust
            font-stretch
*/