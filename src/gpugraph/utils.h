#pragma once

#include <chrono>

namespace gpugraph
{

    class FrameCounter
    {
    public:
        FrameCounter()
            : _time(std::chrono::high_resolution_clock::now())
        {
        }

        bool increase()
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

        std::size_t frames_per_second() const
        {
            return _frames;
        }

    private:
        std::size_t _frames = 0;
        std::size_t _count = 0;
        std::chrono::high_resolution_clock::time_point _time;
    };

}
