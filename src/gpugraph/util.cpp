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
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _time).count() >= 1000)
        {
            _time = now;
            _frames = _count;
            _count = 0;
            return true;
        }
        return false;
    }

    std::size_t FrameCounter::frames_per_second() const
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

}
