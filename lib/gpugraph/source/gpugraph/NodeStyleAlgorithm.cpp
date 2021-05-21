#include "NodeStyleAlgorithm.h"
#include "Style.h"

#include "log.h"

void gpugraph::Node::StyleAlgorithm::operator()(Node& node)
{
    if (node._style == nullptr)
        return;
    
    node._styling = node._style->extract_styling(node);
    log_with_level(9, "applied "<< node._styling.size() << " blocks to \"" << node._style_hash << "\"");
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