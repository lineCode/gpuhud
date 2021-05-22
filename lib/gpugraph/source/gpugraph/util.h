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

#include <chrono>
#include <string>

namespace gpugraph
{

    class FrameCounter
    {
    public:
        FrameCounter();
        bool increase();
        double frames_per_second() const;

    private:
        double _frames = 0;
        std::size_t _count = 0;
        std::chrono::high_resolution_clock::time_point _time;
    };

    class Debounce
    {
    public:
        Debounce(std::chrono::milliseconds = std::chrono::milliseconds(1000));

        template<typename F>
        void operator()(F&&);

    private:
        std::chrono::milliseconds _timeout;
        std::chrono::high_resolution_clock::time_point _time;
    };

    /// read file into memory
    std::string read_file(const char*);

    template<typename F>
    inline void Debounce::operator()(F&& f)
    {
        auto now = std::chrono::high_resolution_clock::now();
        if (now - _time > _timeout)
        {
            _time = now;
            f();
        }
    }

}
