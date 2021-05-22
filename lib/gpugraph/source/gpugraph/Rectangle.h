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

#define GLM_FORCE_XYZW_ONLY
#include <glm/vec2.hpp>

namespace gpugraph
{

    template<typename T>
    struct Rectangle
    {
        Rectangle()
            : p1()
            , p2()
        {
        }

        using point = glm::vec<2, T, glm::defaultp>;
        Rectangle(point p1, point p2)
            : p1(std::move(p1))
            , p2(std::move(p2))
        {
        }

        Rectangle(T x1, T y1, T x2, T y2)
            : p1(x1, y1)
            , p2(x2, y2)
        {
        }

        union
        {
            struct { point p1; };
            struct { T x1; T y1; };
        };

        union
        {
            struct { point p2; };
            struct { T x2; T y2; };
        };

        T width() const;
        T height() const;
    };

    template<typename T>
    T Rectangle<T>::width() const
    {
        return x2 - x1;
    }

    template<typename T>
    T Rectangle<T>::height() const
    {
        return y2 - y1;
    }

    using rect = Rectangle<float>;

}
