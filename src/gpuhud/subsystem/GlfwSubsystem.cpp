#pragma once

#include <stdexcept>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "GlfwSubsystem.h"

namespace gpuhud
{

    namespace
    {
        static void error_callback(int error, const char* description)
        {
            fprintf(stderr, "Error: %s\n", description);
        }
    }

    GlfwSubsystem::GlfwSubsystem()
    {
        if (!glfwInit())
            throw std::runtime_error("could not init subsystem glfw");
        glfwSetErrorCallback(error_callback);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    }

    GlfwSubsystem::~GlfwSubsystem()
    {
        glfwTerminate();
    }

    GlfwSubsystem::GlfwWindow::GlfwWindow(std::string title, std::uint32_t width, std::uint32_t height)
    {
        _window = std::shared_ptr<GLFWwindow>(
            glfwCreateWindow(width, height, title.c_str(), NULL, NULL),
            glfwDestroyWindow);
        if (!_window)
        {
            throw std::runtime_error("failed to create glfw window");
        }
        make_current();
        gladLoadGL();
    }

    void GlfwSubsystem::GlfwWindow::make_current()
    {
        glfwMakeContextCurrent(_window.get());
        glfwSwapInterval(1); // Enable vsync
    }

    void GlfwSubsystem::GlfwWindow::swap_buffers()
    {
        glfwSwapBuffers(_window.get());
    }

    void GlfwSubsystem::GlfwWindow::poll_events()
    {
        glfwPollEvents();
    }

    bool GlfwSubsystem::GlfwWindow::is_closed() const
    {
        return glfwWindowShouldClose(_window.get());
    }

    std::uint32_t GlfwSubsystem::GlfwWindow::width() const
    {
        int w, h;
        glfwGetWindowSize(_window.get(), &w, &h);
        return w;
    }

    std::uint32_t GlfwSubsystem::GlfwWindow::height() const
    {
        int w, h;
        glfwGetWindowSize(_window.get(), &w, &h);
        return h;
    }

    void GlfwSubsystem::GlfwWindow::set_title(std::string const& title)
    {
        glfwSetWindowTitle(_window.get(), title.c_str());
    }

    std::shared_ptr<Subsystem> GlfwSubsystem::instance()
    {
        static std::weak_ptr<Subsystem> instance;
        std::shared_ptr<Subsystem> result = instance.lock();
        if (!result)
            result.reset(new GlfwSubsystem());
        return result;
    }

    std::unique_ptr<Subsystem::Window> GlfwSubsystem::create_window(std::string title, std::uint32_t width, std::uint32_t height)
    {
        return std::make_unique<GlfwSubsystem::GlfwWindow>(std::move(title), width, height);
    }

}
