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

#include "Window.h"

#include <gpugraph/opengl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <chrono>

#include <gpugraph/Intermediate.h>
#include <gpugraph/util.h>
#include <gpugraph/log_level.h>

#include "subsystem/GlfwSubsystem.h"

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

        auto root_node = this->root_node();
        log_with_level(99, "node size: " << sizeof(gpugraph::Node) << "bytes");
        log_with_level(99, "distance size: " << sizeof(css::distance) << "bytes");
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

    void Window::make_root_node_fill_window()
    {
        auto width = _subsystem_window->width();
        auto height = _subsystem_window->height();
        auto& style = root_node()->computed_style_set();
        if (style.contains<gpugraph::ComputedStyle::Width>() && style.contains<gpugraph::ComputedStyle::Height>())
        {
            auto& computed_width = style.get < gpugraph::ComputedStyle::Width>()->value();
            auto& computed_height = style.get < gpugraph::ComputedStyle::Height>()->value();
            if (std::holds_alternative<css::length>(computed_width)
                && std::holds_alternative<css::length>(computed_height)
                && std::get<css::length>(computed_width) == (width | css::length_unit::px)
                && std::get<css::length>(computed_height) == (height |css::length_unit::px))
                return;
        }
        root_node()->set_style(
            "width:" + css::to_string(width|css::length_unit::px) + ";"  +
            "height:" + css::to_string(height|css::length_unit::px));
    }

    void Window::loop()
    {
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
            if(_hot_reload_style)
                hot_reload_style();

            auto width = _subsystem_window->width();
            auto height = _subsystem_window->height();
            /*TODO root_node->set_width(width | Length::px);
            root_node->set_height(height | Length::px);
            */
            make_root_node_fill_window();
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
                log_error("opengl usage is erroneous!")

            if (_frame_counter.increase())
            {
                log_with_level(log_level_renderer, _frame_counter.frames_per_second() << " fps ");
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

    void Window::hot_reload_style()
    {
        if (!_style_filename)
            return;
        _style_debouncer([this]() {
            auto time = fs::last_write_time(_style_filename.value());
            if (time - _style_last_write_time > std::chrono::milliseconds(0))
            {
                _style_last_write_time = time;
                log_info("hot reloading \"" << _style_filename.value() << "\"");
                auto style_collection = std::make_shared<gpugraph::StyleCollection>();
                auto content = gpugraph::read_file(_style_filename.value().c_str());
                try
                {
                    style_collection->compile(content);
                }
                catch (std::runtime_error& e)
                {
                    log_error("exception while loading style: " << e.what());
                }
                root_node()->set_style_collection(style_collection);
            }
        });
    }

    void Window::load_style(std::string const& filename, bool hot_reload)
    {
        if (filename.empty())
        {
            _style_filename.reset();
            return;
        }
        _hot_reload_style = hot_reload;
        _style_filename = filename;
        _style_last_write_time = fs::last_write_time(filename);
        auto content = gpugraph::read_file(filename.c_str());
        auto style_collection = std::make_shared<gpugraph::StyleCollection>();
        style_collection->compile(content);
        root_node()->set_style_collection(style_collection);
    }

}
