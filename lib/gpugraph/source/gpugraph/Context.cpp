#pragma once

#include "Context.h"

#include <stdexcept>

namespace gpugraph
{

    namespace
    {
        thread_local Context* _current_context;
    }

    Context::Context()
        : _skia_context()
    {
        //
        // this was taken from here: https://gist.github.com/ad8e/dd150b775ae6aa4d5cf1a092e4713add
    	auto interface = GrGLMakeNativeInterface();
	    _skia_context.reset(GrDirectContext::MakeGL(interface).release());
    }

    Context& Context::current()
    {
        if (!_current_context)
            throw std::runtime_error("no context active");
        return *_current_context;
    }

    void Context::make_current()
    {
        _current_context = this;
    }

    GrDirectContext& Context::skia_context()
    {
        return *_skia_context;
    }

    SkSurface& Context::skia_surface()
    {
        return *_skia_surface;
    }

    real_t Context::to_real(Length const& l) const
    {
        static auto convert = std::vector<std::function<real_t(Context const &, real_t)>>
        {
            [this](Context const& context, real_t value) {
                return value;
            }
        };
        return convert.at(static_cast<std::size_t>(l.unit))(*this, l.value);
    }

}
