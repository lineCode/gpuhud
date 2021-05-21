#pragma once

#include <functional>
#include <set>
#include <unordered_map>
#include <vector>

namespace gpugraph
{
    
    class Node;

    class Style
    {
    public:
        struct Selector;
        struct Block;

        using Sheet = std::function<void(Node&)>;
        using Styling = std::vector<std::shared_ptr<Block>>;

        void compile(std::string const&);
        Styling extract_linkable_styling_of(Node const&) const;

        std::unordered_map<std::string, std::shared_ptr<Block>>::const_iterator begin() const;
        std::unordered_map<std::string, std::shared_ptr<Block>>::const_iterator end() const;

    private:
        class Compiler;
        std::set<std::shared_ptr<Block>> _blocks;
        std::unordered_map<std::string, std::set<std::shared_ptr<Block>>> _hash;
    };

    struct Style::Block
    {
        Block(std::shared_ptr<Selector>, Styling);

        //
        // a block is linkable, if the every element of the very last 
        // path item is contained in the node style-hash
        bool can_be_linked_to(Node const&);

        std::shared_ptr<Selector> selector;
        Styling styling;
    };

}
