#pragma once

#include "Node.h"

namespace gpugraph
{

    struct Node::StyleAlgorithm
    {
        void link_style_recursively(Node&);
        void apply_linked_styling(Node&);
    };

}
