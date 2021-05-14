#pragma once

#include "types.h"
#include "Node.h"
#include "Rectangle.h"
#include "Context.h" // for skia

namespace gpugraph
{

    using Bounds = Rectangle<float>;

    //
    // sizing is done like:
    // 1) calculating min-content, max-content and content
    //    for width and height
    // 2) adjusting the tree
    // 3) ...
    struct Node::State
    {
        // bound's can be greater than the width/height
        Bounds bounds;

        real_t x = 0; // [px]
        real_t y = 0; // [px]
        real_t width = 0; // [px]
        real_t height = 0; // [px]

        real_t width_content; // [px]
        real_t width_min_content; // [px]
        real_t width_max_content; // [px]

        real_t height_content; // [px]
        real_t height_min_content; // [px]
        real_t height_max_content; // [px]

        std::shared_ptr<SkFont> font;

        void reset();
    };

}
