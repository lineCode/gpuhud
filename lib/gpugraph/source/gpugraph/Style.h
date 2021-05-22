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

        using Rule = std::function<void(Node&)>;
        using Styling = std::vector<std::shared_ptr<Block>>;

        void compile(std::string const&);
        Styling extract_linkable_styling_for(Node const&) const;

        std::unordered_map<std::string, std::shared_ptr<Block>>::const_iterator begin() const;
        std::unordered_map<std::string, std::shared_ptr<Block>>::const_iterator end() const;

    private:
        class Compiler;
        std::set<std::shared_ptr<Block>> _blocks;
        std::unordered_map<std::string, std::set<std::shared_ptr<Block>>> _hash;
    };

}
