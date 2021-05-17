#include "NodeState.h"

namespace gpugraph
{

    void Node::State::reset()
    {
        *this = Node::State();
    }

}
