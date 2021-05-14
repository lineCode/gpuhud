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

    /// read file into memory
    std::string read_file(const char*);

}
