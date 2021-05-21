#pragma once

#include <memory>
#include <string>
#include <cstdint>
#include <functional>
#include <filesystem>

#include <gpugraph/Context.h>
#include <gpugraph/Style.h>
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
