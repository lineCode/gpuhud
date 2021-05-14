#pragma once

#include "RenderTarget.h"

namespace gpugraph
{

    class SkiaRenderTarget : public RenderTarget
    {
    public:
        class SkiaTile : public RenderTarget::Tile
        {
        public:
            SkiaTile(RenderTarget *, std::size_t base_index, rect);
        };

        SkiaRenderTarget(
            std::size_t width=0, 
            std::size_t height=0,
            std::size_t tile_width=256,
            std::size_t overlap=1);

    protected:
        std::unique_ptr<Tile> create_tile(std::size_t base_index, rect) override;
    };

}
