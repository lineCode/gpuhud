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
