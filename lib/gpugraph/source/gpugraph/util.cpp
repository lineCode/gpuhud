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
#include "util.h"

#include <fstream>

namespace gpugraph
{

    FrameCounter::FrameCounter()
        : _time(std::chrono::high_resolution_clock::now())
    {
    }

    bool FrameCounter::increase()
    {
        ++_count;
        auto now = std::chrono::high_resolution_clock::now();
        auto gone = std::chrono::duration_cast<std::chrono::milliseconds>(now - _time).count();
        if (gone >= 1000)
        {
            _time = now;
            _frames = _count / (gone / 1000.0);
            _count = 0;
            return true;
        }
        return false;
    }

    double FrameCounter::frames_per_second() const
    {
        return _frames;
    }

    std::string read_file(const char* filename) 
    {
        std::ifstream file(filename, std::ios_base::binary);
        std::string buffer;
        if (!file.good())
            throw std::invalid_argument(
                std::string("file ") + filename + " doesn't exists");
        file.seekg(0, std::ios_base::end);
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios_base::beg);
        buffer.resize(static_cast<std::size_t>(size));
        file.read(&buffer[0], size);
        return buffer;
    }

    Debounce::Debounce(std::chrono::milliseconds milliseconds)
        : _timeout(milliseconds)
        , _time(std::chrono::high_resolution_clock::now())
    {
    }

}
