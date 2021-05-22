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
        StyleCompiler compiler([this](auto block) {
            _hash[block->selector->path().back().style_hash.key()].insert(block);
        });
        css::parser().parse(source, compiler);
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
                if(block->can_be_linked_to(node))
                    linkable_blocks.push_back(block);
        }
        std::sort(linkable_blocks.begin(), linkable_blocks.end(), [](auto& a, auto& b) {
            return a->selector->specificity() < b->selector->specificity();
        });
        return linkable_blocks;
    }

}
