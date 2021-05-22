#include "NodeState.h"

namespace gpugraph
{

    NodeState::Defaulted NodeState::defaults;

    void NodeState::inherit(NodeState& state)
    {
        // TODO: memcpy?
        inherited = state.inherited;
    }

    void NodeState::reset()
    {
        // TODO: memcpy?
        defaulted = defaults;
    }

}
