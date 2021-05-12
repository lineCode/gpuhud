#pragma once

#include <memory>
#include <string>
#include <cstdint>
#include <functional>

#include "Subsystem.h"

namespace gpuhud
{
    
    class Window 
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

    private:
        std::string _title;
        std::shared_ptr<Subsystem> _subsystem;
        std::unique_ptr<Subsystem::Window> _subsystem_window;
    };

}
