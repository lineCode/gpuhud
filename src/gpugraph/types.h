#pragma once

#define GLM_FORCE_XYZW_ONLY
#include <glm/vec2.hpp>
#include <glm/mat3x3.hpp>
#undef GLM_CONFIG_XYZW_ONLY
#include <glm/vec4.hpp>

#include "Rectangle.h"

namespace gpugraph
{

    using vec2 = glm::vec2;
    using vec4 = glm::vec4;
    using mat3x3 = glm::mat3x3;
    using color = glm::vec4;
    using real_t = float;

}
