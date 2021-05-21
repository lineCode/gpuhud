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

    Style::Styling Style::extract_styling(Node const& node) const
    {
        std::vector<std::shared_ptr<Block>> extracted_blocks;
        auto node_style_hash = node.style_hash().values();
        node_style_hash.insert(std::string());
        log_debug("style node of type \"" << node.type() << "\" with \"" << node.style_hash() << "\"")
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
                if(block->is_applicable_to(node))
                    extracted_blocks.push_back(block);
        }
        std::sort(extracted_blocks.begin(), extracted_blocks.end(), [](auto& a, auto& b) {
            return a->selector->specificity() < b->selector->specificity();
        });
        return extracted_blocks;
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

    bool Style::Block::is_applicable_to(Node const& node)
    {
        return selector->path().back().style_hash.is_subset_of(node.style_hash());
    }

}
