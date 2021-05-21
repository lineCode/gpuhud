#pragma once

#include "Widget.h"

namespace gpuhud
{

    //
    // this node can just load and display pseudo-html
    class NodeTree : public Widget
    {
    public:
        NodeTree(std::string const& filename);
    };

}
