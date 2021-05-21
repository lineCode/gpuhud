#include "StyleCompiler.h"

#include <css/parser.hpp>
#include "StyleSelector.h"
#include "Node.h"

#include "log.h"

namespace gpugraph
{

    void Style::compile(std::string const& source)
    {
        Compiler compiler(*this);
        css::parser().parse(source, compiler);
    }

    Style::Styling Style::extract_linkable_styling_of(Node const& node) const
    {
        std::vector<std::shared_ptr<Block>> linkable_blocks;
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

    Style::Block::Block(std::shared_ptr<Selector> selector, Styling styling)
        : selector(std::move(selector))
        , styling(std::move(styling))
    {
    }

    namespace
    {
        std::string stringify(std::set<std::string> const& hash)
        {
            std::string str;
            for (auto& element : hash)
                str += element;
            return str;
        }
    }

    bool Style::Block::can_be_linked_to(Node const& node)
    {
        return selector->path().back().style_hash.is_subset_of(node.style_hash());
    }

}
