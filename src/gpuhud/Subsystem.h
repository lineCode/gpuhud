#pragma once

#include <memory>
#include <string>
#include <cstdint>
#include <functional>

namespace gpuhud
{

    class Subsystem
    {
    public:
        class Window;
        virtual ~Subsystem() = default;
        virtual std::unique_ptr<Window> create_window(
            std::string title,
            std::uint32_t width,
            std::uint32_t height) = 0;

    private:
        int ver = 0;
    };

    class Subsystem::Window
    {
    public:
        using UpdateCallback = std::function<void(std::shared_ptr<Window>)>;

        virtual ~Window() = default;
        virtual void set_title(std::string const&) = 0;

        virtual bool is_closed() const = 0;

        virtual std::uint32_t width() const = 0;
        virtual std::uint32_t height() const = 0;

        virtual void make_current() = 0;
        virtual void swap_buffers() = 0;
        virtual void poll_events() = 0;

        virtual double get_dpi() const = 0;
    };

}