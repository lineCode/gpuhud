#pragma once

#include <memory>
#include <string>
#include <cstdint>
#include <functional>

#include "Subsystem.h"
#include "Container.h"

namespace gpuhud
{
    
    class Window : public Container
    {
    public:
        Window(
            std::uint32_t width=1024, 
            std::uint32_t height=768, 
            std::shared_ptr<Subsystem> subsystem=nullptr);

        void loop();

        const std::string& title() const;
        void Window::set_title(std::string);

        std::uint32_t width() const;
        std::uint32_t height() const;

        void set_debug_draw_layout(bool);
        bool debug_draw_layout() const;

        void set_debug_draw_intermediate(bool);
        bool debug_draw_intermediate() const;

    private:
        std::string _title;
        std::shared_ptr<Subsystem> _subsystem;
        std::unique_ptr<Subsystem::Window> _subsystem_window;

        bool _debug_draw_layout = true;
        bool _debug_draw_intermediate = true;

        std::size_t _frame_count=0;
        float _fps;
    };

}