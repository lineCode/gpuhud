#include "SkiaRenderTarget.h"

#include <stdexcept>
#pragma warning(push)
#pragma warning(disable: 4244)
#pragma warning(disable: 4081)
#pragma warning(disable: 4267)
#include <skia.h>
#pragma warning(pop)

#include "Context.h"

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
        auto& context = Context::current().skia_context();


	    GrGLFramebufferInfo framebufferInfo;
        framebufferInfo.fFBOID = render_target->framebuffer_objects().at(base_index);
        framebufferInfo.fFormat = GL_RGBA8;

	    SkColorType colorType = kRGBA_8888_SkColorType;
	    GrBackendRenderTarget backendRenderTarget(
            static_cast<int>(rectangle().width()), 
            static_cast<int>(rectangle().height()), 
		    0, // sample count
		    0, // stencil bits
		    framebufferInfo);

	    //(replace line below with this one to enable correct color spaces) sSurface = SkSurface::MakeFromBackendRenderTarget(sContext, backendRenderTarget, kBottomLeft_GrSurfaceOrigin, colorType, SkColorSpace::MakeSRGB(), nullptr).release();
	    _surface.reset(SkSurface::MakeFromBackendRenderTarget(&context, backendRenderTarget, kBottomLeft_GrSurfaceOrigin, colorType, nullptr, nullptr).release());
	    if (!_surface) 
            throw std::runtime_error("failed to create skia surface");
    }

    void SkiaRenderTarget::SkiaTile::render(std::function<void()> f)
    {
        auto& context = Context::current();
        context._skia_surface = _surface.get();
        Tile::render(std::move(f));
        context.skia_context().flush();
   }

}
