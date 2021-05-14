#include "Window.h"

#include <gpugraph/opengl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "subsystem/GlfwSubsystem.h"

#include <gpugraph/Intermediate.h>

void GLAPIENTRY
MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
}

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
        // glEnable( GL_DEBUG_OUTPUT );
        // glDebugMessageCallback( MessageCallback, 0 );
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
            root_node->set_width(width | px);
            root_node->set_height(height | px);
            root_node->intermediate()->set_size(width, height);

            for (auto& tile : root_node->intermediate()->render_target().tiles())
            {
                tile->render([&]() {
                    auto w = tile->rectangle().width();
                    auto h = tile->rectangle().height();
                    glMatrixMode(GL_PROJECTION);
                    glPushMatrix();
                    glLoadIdentity();
                    glOrtho(0, w, h, 0, 1., -1.);
                    glMatrixMode(GL_MODELVIEW);
                    glPushMatrix();
                    glLoadIdentity();

                    glBegin(GL_TRIANGLES);
                    glColor3f(1.0f, 0.0f, 0.0f); glVertex2f(w-100.f, 50.f);
                    glColor3f(0.0f, 1.0f, 0.0f); glVertex2f(w-50.f, 50.f);
                    glColor3f(0.0f, 0.0f, 1.0f); glVertex2f(w-50.f, 100.f);
                    glEnd();

                    glPopMatrix();
                    glMatrixMode(GL_PROJECTION);
                    glPopMatrix();
                    glMatrixMode(GL_MODELVIEW);
                });
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, width, height);
            glClearColor(0.0, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (_debug_draw_intermediate)
                _draw_intermediate();

            // will move this ..
            auto& intermediate = *this->root_node()->intermediate();
            auto mat = glm::ortho(
                static_cast<float>(0),
                static_cast<float>(width),
                static_cast<float>(height),
                static_cast<float>(0)
            );
            intermediate.render_target().blit(mat);


            _subsystem_window->swap_buffers();
            _subsystem_window->poll_events();

            while (glGetError() != GL_NO_ERROR)
                std::cerr << "opengl usage is erroneous, debug it!" << std::endl;

            if (_frame_counter.increase())
            {
                std::cout << _frame_counter.frames_per_second() << " fps "
                << "(" << root_node->intermediate()->render_target().tiles().size() << " tiles)" << std::endl;
            }
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

    void Window::_draw_intermediate(bool)
    {
        auto width = _subsystem_window->width();
        auto height = _subsystem_window->height();

        auto& intermediate = *this->root_node()->intermediate();

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, width, height, 0, 1., -1.);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        intermediate._debug_draw(true);
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }


}
