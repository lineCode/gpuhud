#pragma once

#include "Widget.h"
#include <vector>
#include <memory>

namespace gpuhud
{

    class Container : public Widget
    {
    public:
        void add(std::shared_ptr<Widget>);
        void remove(std::shared_ptr<Widget>);
        void clear();

    protected:
        void set_content_node(std::shared_ptr<gpugraph::Node>);

    private:
        Widget* _parent;
        std::shared_ptr<gpugraph::Node> _content_node;
        std::vector<std::shared_ptr<Widget>> _children;
    };

}
