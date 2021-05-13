#include "Window.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "subsystem/GlfwSubsystem.h"

namespace gpuhud
{

    Window::Window(std::uint32_t width, std::uint32_t height, std::shared_ptr<Subsystem> subsystem)
        : _subsystem(subsystem ? _subsystem : GlfwSubsystem::instance())
        , _subsystem_window(_subsystem->create_window("gpuhud", width, height))
    {
        auto node = std::make_shared<gpugraph::Node>();
        set_root_node(node);
        set_content_node(node);
    }

    void Window::loop()
    {
        using gpugraph::px;

        auto& node = this->root_node();
        
        node->set_width(_subsystem_window->width()|px);
        node->set_height(_subsystem_window->height()|px);

        _subsystem_window->make_current();
        while (!_subsystem_window->is_closed())
        {
            glClearColor(0.0, 1.0, 0.0, 0.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // #1 update node-tree
            // #2 render composition
            _subsystem_window->swap_buffers();
            _subsystem_window->poll_events();
        }
    }

    void Window::set_title(std::string title)
    {
        _subsystem_window->set_title(title);
        _title = std::move(title);
    }

    const std::string& Window::title() const
    {
        return _title;
    }

    std::uint32_t Window::width() const
    {
        return _subsystem_window->width();
    }

    std::uint32_t Window::height() const
    {
        return _subsystem_window->height();
    }

}
