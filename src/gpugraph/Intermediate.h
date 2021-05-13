#pragma once

#include <memory>
#include "RenderTarget.h"

namespace gpugraph
{

    /// an intermediate rendering of a subtree
    class Intermediate
    {
    public:
        void set_size(std::size_t width, std::size_t height);
        std::array<std::size_t, 2> size();

        void _debug_draw(bool recursive=true);

        RenderTarget& render_target();

    private:
        Intermediate* parent;
        std::shared_ptr<Intermediate> _children;
        RenderTarget _render_target;
    };

}
