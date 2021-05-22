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

#include <memory>
#include <string>
#include <cstdint>
#include <functional>
#include <filesystem>

#include <gpugraph/Context.h>
#include <gpugraph/StyleCollection.h>
#include <gpugraph/util.h>

#include "Subsystem.h"
#include "Container.h"

namespace fs = std::filesystem;

namespace gpuhud
{
    
    class Window : public Container
    {
    public:
        Window(
            std::uint32_t width=1024, 
            std::uint32_t height=768, 
            std::shared_ptr<Subsystem> subsystem=nullptr);

        void load_style(std::string const& filename, bool hot_reload=true);
        void loop();

        const std::string& title() const;
        void set_title(std::string);

        std::uint32_t width() const;
        std::uint32_t height() const;

        void set_debug_draw_layout(bool);
        bool debug_draw_layout() const;

        void set_debug_draw_intermediate(bool);
        bool debug_draw_intermediate() const;

    private:
        void hot_reload_style();
        bool _hot_reload_style = false;
        std::optional<std::string> _style_filename;
        gpugraph::Debounce _style_debouncer;
        std::filesystem::file_time_type _style_last_write_time;

        std::string _title;
        std::shared_ptr<Subsystem> _subsystem;
        std::unique_ptr<Subsystem::Window> _subsystem_window;
        
        gpugraph::Context _context;
        gpugraph::FrameCounter _frame_counter = gpugraph::FrameCounter();
    };

}
