#pragma once

#define GLM_CONFIG_XYZW_ONLY
#include <glm/vec2.hpp>
#include <glm/mat3x3.hpp>
#undef GLM_CONFIG_XYZW_ONLY
#include <glm/vec4.hpp>

namespace gpugraph
{

using vec2 = glm::vec2;
using vec4 = glm::vec4;
using mat3x3 = glm::mat3x3;
using color = glm::vec4;
using real_t = float;

struct rect
{
    rect(vec2 p1, vec2 p2)
        : p1(std::move(p1))
        , p2(std::move(p2))
    {
    }

    rect(real_t x, real_t y, real_t width, real_t height)
        : p1(x, y)
        , p2(x + width, y + height)
    {
    }

    union
    {
        struct { vec2 p1; };
        struct { real_t x1; real_t y1;};
    };

    union
    {
        struct { vec2 p2; };
        struct { real_t x2; real_t y2;};
    };
};

}
