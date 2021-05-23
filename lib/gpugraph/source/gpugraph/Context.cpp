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

}
