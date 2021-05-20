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
