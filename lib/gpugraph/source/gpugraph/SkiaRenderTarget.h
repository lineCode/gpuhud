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
