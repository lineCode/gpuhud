#pragma once

#include "RenderTarget.h"
#include "Context.h"

namespace gpugraph
{

    class SkiaRenderTarget : public RenderTarget
    {
    public:
        class SkiaTile : public RenderTarget::Tile
        {
        public:
            SkiaTile(RenderTarget *, std::size_t base_index, rect);
        
            void render(std::function<void()> f) override;

        private:
            SkPaint _debug_intermediate_paint;
            std::unique_ptr<SkSurface> _surface;
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
