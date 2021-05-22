/***************************************************************************//*/

Copyright (c) 2021 Martin Rudoff

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

/******************************************************************************/
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
            log_with_level(9, "apply \"" << block->selector()->definition() << "\" to node \"" << node._style_hash << "\"");
            for (auto& rule : block->rules())
                rule(node);
        }
    }
    if(node._style) for (auto& rule : node._style->rules())
        rule(node);
}
