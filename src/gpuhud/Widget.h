#pragma once

#include <memory>
#include <vector>

namespace gpuhud
{

    class Widget
    {
    public:
        Widget();

    private:
        std::vector<std::shared_ptr<Widget>> _children;
    };

}