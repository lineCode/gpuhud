#include "Window.h"

#include <gpugraph/opengl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "subsystem/GlfwSubsystem.h"

#include <gpugraph/Intermediate.h>
#include <gpugraph/attributes.h>

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
        , _context()
    {
        _context.make_current();

        using gpugraph::Length;
        std::cout << "size: " << sizeof(100 | Length::px) << std::endl;
        
        auto root_node = this->root_node();
        /* TODO
        root_node->set_left(0 | Length::px);
        root_node->set_top(0 | Length::px);
        root_node->set_width(width | Length::px);
        root_node->set_height(height | Length::px);
        root_node->set_position(gpugraph::Position::Absolute);
        */

        set_content_node(root_node);
        root_node->set_force_intermediate(true);
        // glEnable( GL_DEBUG_OUTPUT );
        // glDebugMessageCallback( MessageCallback, 0 );
    }

    void Window::loop()
    {
        using gpugraph::Length;

        auto& root_node = this->root_node();
        _subsystem_window->make_current();

        auto dpi = _subsystem_window->get_dpi();
        auto tf = SkTypeface::MakeFromName("Lucida Console", SkFontStyle::Normal());
        auto default_font = std::make_shared<SkFont>(tf);
        default_font->setSize(static_cast<float>(std::round(dpi / 5.f))); // "dpi == dots per 25,4[mm]"
        //TODO root_node->set_font(default_font);

        bool first = true;
        while (!_subsystem_window->is_closed())
        {
            auto width = _subsystem_window->width();
            auto height = _subsystem_window->height();
            /*TODO root_node->set_width(width | Length::px);
            root_node->set_height(height | Length::px);
            */
            root_node->intermediate()->set_size(width, height);

            auto const& tiles = root_node->intermediate()->render_target().tiles();

            static float deg = 0.f;
            deg += 3.0f;
            for (auto& tile : tiles)
            {
                tile->render([&]() {
                    auto& surface = gpugraph::Context::current().skia_surface();
                    SkCanvas* canvas = surface.getCanvas(); // We don't manage this pointer's lifetime.
                    SkPaint paint;
                    canvas->clear(SK_ColorWHITE);
                    paint.setColor(SK_ColorCYAN);
                    {
                        SkPaint paint;
                        paint.setAntiAlias(true);
//tODO                        canvas->drawSimpleText("GpuHud", 6, SkTextEncoding::kUTF8, 200, 100, *root_node->font(), paint);
                    }

                    canvas->rotate(deg, static_cast<SkScalar>(width / 2), static_cast<SkScalar>(height / 2));
                    canvas->drawRect({ 300, 300,
                        static_cast<float>(width - 300), static_cast<float>(height - 300) }, paint);
                });
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            glViewport(0, 0, width, height);
            glPushAttrib(GL_COLOR_BUFFER_BIT);
            glClearColor(1.0, 1.0f, 1.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glPopAttrib();
            auto& intermediate = *this->root_node()->intermediate();
            auto mat = glm::ortho(
                static_cast<float>(0),
                static_cast<float>(width),
                static_cast<float>(height),
                static_cast<float>(0)
            );
            glDisable(GL_DEPTH_TEST);
            intermediate.render_target().blit(mat);

            /*
            if (_debug_draw_intermediate)
                _draw_intermediate();*/

            _subsystem_window->swap_buffers();
            _subsystem_window->poll_events();

            while (glGetError() != GL_NO_ERROR)
                std::cerr << "opengl usage is erroneous, debug it!" << std::endl;

            /*
            if (_frame_counter.increase())
            {
                std::cout << _frame_counter.frames_per_second() << " fps "
                    << "(" << root_node->intermediate()->render_target().tiles().size() << " tiles)" << std::endl;
            }*/
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
