#pragma once

#include "Node.h"

namespace gpugraph
{

    struct Node::StyleAlgorithm
    {
        void operator()(Node&);
    };

}
