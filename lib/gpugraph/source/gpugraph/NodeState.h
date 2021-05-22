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