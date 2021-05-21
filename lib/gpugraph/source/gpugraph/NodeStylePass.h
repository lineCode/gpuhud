#pragma once

#include "Node.h"

namespace gpugraph
{

    struct Node::StylePass
    {
        void operator()(Node&);
    };

}
