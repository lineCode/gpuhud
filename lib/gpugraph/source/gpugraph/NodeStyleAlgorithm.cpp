#include "NodeStyleAlgorithm.h"
#include "Style.h"

#include "log.h"

void gpugraph::Node::StyleAlgorithm::link_style_recursively(Node& node)
{
    if (node._style == nullptr)
        return;
    
    node._styling = node._style->extract_linkable_styling_of(node);
    //
    // way down
    log_with_level(9, "linked "<< node._styling.size() << " blocks to \"" << node._style_hash << "\"");
    for (auto& child : node._children)
    {
        child->_style = node._style;
        this->link_style_recursively(*child);
    }
    //
    // way back during depth first traversal
    apply_linked_styling(node);
}

void gpugraph::Node::StyleAlgorithm::apply_linked_styling(Node& node)
{
    // 1) apply
    for (auto& block : node._styling)
    {
    }
}

/*
a) Node: create set
b) apply
c) block: "is active" (selector)

1) content auf dem rückweg berechnen
2) content vom verursacher zur wurzel hin berechnen

bei 1 u. 2 entsprechend "render" oder "resized" angeben
*/