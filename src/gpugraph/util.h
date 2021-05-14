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
        std::size_t frames_per_second() const;

    private:
        std::size_t _frames = 0;
        std::size_t _count = 0;
        std::chrono::high_resolution_clock::time_point _time;
    };

    std::string read_file(const char*);

}
