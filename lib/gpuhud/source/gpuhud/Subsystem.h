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