#pragma once

#include <gpuhud/Subsystem.h>

struct GLFWwindow;

namespace gpuhud
{

    class GlfwSubsystem 
        : public Subsystem
        , public std::enable_shared_from_this<GlfwSubsystem>
    {
    public:
        class GlfwWindow;

        static std::shared_ptr<Subsystem> instance();
        
        std::unique_ptr<Window> create_window(
            std::string title, 
            std::uint32_t width, 
            std::uint32_t height) override;

        ~GlfwSubsystem();

    private:
        GlfwSubsystem();
    };    

    class GlfwSubsystem::GlfwWindow
        : public Subsystem::Window
    {
    public:
        GlfwWindow(std::string title, std::uint32_t width, std::uint32_t height);

        void make_current() override;
        void swap_buffers() override;
        void poll_events() override;
        
        bool is_closed() const override;
        
        std::uint32_t width() const override;
        std::uint32_t height() const override;
        
        void set_title(std::string const&) override;

    private:
        mutable std::shared_ptr<GLFWwindow> _window;
    };

}