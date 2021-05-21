#include "NodeStyleAlgorithm.h"
#include "Style.h"

void gpugraph::Node::StyleAlgorithm::operator()(Node& node)
{
    if (node._style == nullptr)
        return;
    
    auto styling = node._style->extract_styling(node);

    for (auto& child : node._children)
    {
        child->_style = node._style;
        (*this)(*child);
    }

    //
    // TODO:
}

/*
a) Node: create set
b) apply
c) block: "is active" (selector)

1) content auf dem rückweg berechnen
2) content vom verursacher zur wurzel hin berechnen

bei 1 u. 2 entsprechend "render" oder "resized" angeben
*/