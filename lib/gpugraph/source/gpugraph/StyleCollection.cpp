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

#include "StyleCollection.h"

#include <css/parser.hpp>

#include "StyleCompiler.h"
#include "StyleSelector.h"
#include "StyleBlock.h"
#include "Node.h"
#include "log.h"

namespace gpugraph
{

    void StyleCollection::compile(std::string const& source)
    {
        StyleCompiler([this](auto block) {
            _hash[block->selector()->path().back().style_hash.key()].insert(block);
        }).compile(source);
    }

    Styling StyleCollection::extract_linkable_styling_for(Node const& node) const
    {
        std::vector<std::shared_ptr<StyleBlock>> linkable_blocks;
        auto node_style_hash = node.style_hash().values();
        node_style_hash.insert(std::string());
        //
        // go through all "identifiers" of the node
        for (auto& identifier : node_style_hash)
        {
            auto it = _hash.find(identifier);
            if (it == _hash.end())
                continue;
            //
            // each block/entry in the hashed block-set is a valid candidate
            for (auto& block : it->second)
                if(block->is_linkable_to(node))
                    linkable_blocks.push_back(block);
        }
        std::sort(linkable_blocks.begin(), linkable_blocks.end(), [](auto& a, auto& b) {
            return a->selector()->specificity() < b->selector()->specificity();
        });
        return linkable_blocks;
    }

}
