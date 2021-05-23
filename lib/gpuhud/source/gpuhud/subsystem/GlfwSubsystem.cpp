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

    double GlfwSubsystem::GlfwWindow::get_dpi() const
    {
        // this was party taken from here:
        // https://www.glfw.org/docs/3.0/monitor.html
        auto monitor = glfwGetWindowMonitor(_window.get());
        if (monitor == nullptr)
            monitor = glfwGetPrimaryMonitor();
        int width, height;
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwGetMonitorPhysicalSize(monitor, &width, &height);
        // 25,4 = one inch in [mm]
        return mode->width / (width / 25.4);
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
