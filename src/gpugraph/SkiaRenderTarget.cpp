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

        auto x1 = rectangle().x1;
        auto y1 = rectangle().y1;
        auto overlap = render_target->overlap();
	    
        SkColorType colorType = kRGBA_8888_SkColorType;
	    GrBackendRenderTarget backendRenderTarget(
            static_cast<int>(rectangle().width()), 
            static_cast<int>(rectangle().height()),
		    0, // sample count (multi-sampled)
		    0, // stencil bits
		    framebufferInfo);

	    //(replace line below with this one to enable correct color spaces) sSurface = SkSurface::MakeFromBackendRenderTarget(sContext, backendRenderTarget, kBottomLeft_GrSurfaceOrigin, colorType, SkColorSpace::MakeSRGB(), nullptr).release();
	    _surface.reset(SkSurface::MakeFromBackendRenderTarget(&context, backendRenderTarget, kTopLeft_GrSurfaceOrigin, colorType, nullptr, nullptr).release());
        _surface->getCanvas()->translate(-x1, -y1);
            //((y2 - render_target->size()[1])));
	    if (!_surface) 
            throw std::runtime_error("failed to create skia surface");
    }

    void SkiaRenderTarget::SkiaTile::render(std::function<void()> f)
    {
        auto fbo_id = _render_target->framebuffer_objects().at(_base_index);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
        glViewport(0, 0,
            static_cast<GLsizei>(_rectangle.width()),
            static_cast<GLsizei>(_rectangle.height()));
        auto& context = Context::current();
        context._skia_surface = _surface.get();
        _surface->getCanvas()->save();
        _surface->getCanvas()->clear(SK_ColorWHITE);
        f();
        _surface->getCanvas()->restore();
        context.skia_context().flush();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
   }

}
