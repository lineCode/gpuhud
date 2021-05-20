#include "NodeState.h"

namespace gpugraph
{

    Node::State::Defaulted Node::State::defaults;

    void Node::State::inherit(State& state)
    {
        // TODO: memcpy?
        inherited = state.inherited;
    }

    void Node::State::reset()
    {
        // TODO: memcpy?
        defaulted = defaults;
    }

}
