#include "Window.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "subsystem/GlfwSubsystem.h"

#include <gpugraph/Intermediate.h>

namespace gpuhud
{

    Window::Window(std::uint32_t width, std::uint32_t height, std::shared_ptr<Subsystem> subsystem)
        : _subsystem(subsystem ? _subsystem : GlfwSubsystem::instance())
        , _subsystem_window(_subsystem->create_window("gpuhud", width, height))
    {
        auto root_node = std::make_shared<gpugraph::Node>();
        set_root_node(root_node);
        set_content_node(root_node);
        root_node->set_force_intermediate(true);
    }

    void Window::loop()
    {
        using gpugraph::px;

        auto& root_node = this->root_node();
        
        _subsystem_window->make_current();
        while (!_subsystem_window->is_closed())
        {
            auto width = _subsystem_window->width();
            auto height = _subsystem_window->height();
            root_node->set_width(width|px);
            root_node->set_height(height|px);
            root_node->intermediate()->set_size(width, height);

            glViewport(0, 0, width, height);
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            if (_debug_draw_intermediate)
            {
                glMatrixMode(GL_MODELVIEW_MATRIX);
                glPushMatrix();
                glLoadIdentity();
                glMatrixMode(GL_PROJECTION_MATRIX);
                glPushMatrix();
                glLoadIdentity();
                glScalef(1.f, -1.f, 1.f);
                glOrtho( 0, width, 0, height, -1, 1 );
                root_node->intermediate()->_debug_draw(true);
                glPopMatrix();
                glMatrixMode(GL_MODELVIEW_MATRIX);
                glPopMatrix();
            }
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

    void Window::set_debug_draw_layout(bool value)
    {
        _debug_draw_layout = value;
    }

    bool Window::debug_draw_layout() const
    {
        return _debug_draw_layout;
    }

    void Window::set_debug_draw_intermediate(bool value)
    {
        _debug_draw_intermediate = value;
    }

    bool Window::debug_draw_intermediate() const
    {
        return _debug_draw_intermediate;
    }

}
