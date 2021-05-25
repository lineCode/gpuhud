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
        struct
        {
            bool visible = true;
            bool active = false;
            bool hovered = false;
            bool focused = false;

            Bounds border_box;
            Bounds content_box;

            bool force_layout = true;
            bool force_redraw = true;
        } state;
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