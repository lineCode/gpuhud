#pragma once

#include <functional>

namespace gpugraph
{

    class Node;

    using StyleRule = std::function<void(Node&)>;

}
