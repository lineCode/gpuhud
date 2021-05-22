#include "NodeStyleAlgorithm.h"
#include "StyleCollection.h"
#include "StyleBlock.h"
#include "StyleSelector.h"

#include "log.h"

void gpugraph::Node::StyleAlgorithm::link_style_recursively(Node& node)
{
    if (node._style_collection == nullptr)
        return;
    
    node._styling = node._style_collection->extract_linkable_styling_for(node);
    //
    // way down
    log_with_level(9, "linked "<< node._styling.size() << " blocks to \"" << node._style_hash << "\"");
    //
    // apply style to state
    apply_linked_styling(node);

    for (auto& child : node._children)
    {
        child->_style_collection = node._style_collection;
        this->link_style_recursively(*child);
    }
    //
    // way back during depth first traversal
    // TODO: .. compute content ..
}

void gpugraph::Node::StyleAlgorithm::apply_linked_styling(Node& node)
{
    // 1) apply
    for (auto& block : node._styling)
    {
        if (block->is_applicable_to(node))
        {
            log_with_level(9, "apply \""<< block->selector()->definition() << "\" to node \"" << node._style_hash << "\"");
            for (auto& rule : block->rules())
                rule(node);
        }
    }
    if(node._style) for (auto& rule : node._style->rules())
        rule(node);
}
