#pragma once

#include "Container.h"

namespace gpuhud
{

    class ChildWindow : public Container
    {
    public:
        ChildWindow();

    private:
        std::shared_ptr<gpugraph::Node> _header_row;
        std::shared_ptr<gpugraph::Node> _title_label;
        std::shared_ptr<gpugraph::Node> _close_button;
    };

}
