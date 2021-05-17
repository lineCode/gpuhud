#include "Intermediate.h"

namespace gpugraph
{

    void Intermediate::_debug_draw(bool recursive)
    {
        _render_target._debug_draw();
    }

    RenderTarget& Intermediate::render_target()
    {
        return _render_target;
    }

    void Intermediate::set_size(std::size_t width, std::size_t height)
    {
        _render_target.set_size(width, height);
    }

    std::array<std::size_t, 2> Intermediate::size()
    {
        return _render_target.size();
    }


}