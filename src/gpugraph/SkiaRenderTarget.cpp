#include "SkiaRenderTarget.h"

#pragma warning(push)
#pragma warning(disable: 4244)
#pragma warning(disable: 4081)
#pragma warning(disable: 4267)
#include <skia.h>
#pragma warning(pop)

namespace gpugraph
{

    SkiaRenderTarget::SkiaRenderTarget(std::size_t width, std::size_t height, std::size_t tile_width, std::size_t overlap)
        : RenderTarget(width, height, tile_width, overlap)
    {
    }

    std::unique_ptr<RenderTarget::Tile> SkiaRenderTarget::create_tile(std::size_t base_index, rect rect)
    {
        return std::make_unique<SkiaTile>(this, base_index, std::move(rect));
    }

    SkiaRenderTarget::SkiaTile::SkiaTile(RenderTarget* render_target, std::size_t base_index, rect r)
        : Tile(render_target, base_index, std::move(r))
    {
    }

}
