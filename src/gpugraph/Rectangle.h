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
