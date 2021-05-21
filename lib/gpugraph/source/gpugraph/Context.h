#pragma once

#pragma warning(push)
#pragma warning(disable: 4244)
#pragma warning(disable: 4081)
#pragma warning(disable: 4267)
#define SK_GL
#include <skia.h>
#pragma warning(pop)

namespace gpugraph
{

    class Context
    {
    public:
        Context();

        // get active context for the current thread
        static Context& current();

        // make this context the active one for the current thread
        void make_current();

        // get skia context
        GrDirectContext& skia_context();

        // get skia surface
        SkSurface& skia_surface();

    private:
        friend class SkiaRenderTarget;
        SkSurface* _skia_surface;
        std::unique_ptr<GrDirectContext> _skia_context;

        bool _debug_intermediate = true;
        bool _debug_layout = false;
    };

}
