#pragma once

#include <functional>
#include <unordered_map>

#include "Node.h"

namespace gpugraph
{
    
    class Style
    {
    public:
        struct Selector;
        struct Block;
        using Sheet = std::function<void(Node::State&, Node&)>;
        using Styling = std::vector<std::shared_ptr<Sheet>>;

        void compile(std::string const&);
        Styling make_styling(Node const&) const;

        std::unordered_map<std::string, std::shared_ptr<Block>>::const_iterator begin() const;
        std::unordered_map<std::string, std::shared_ptr<Block>>::const_iterator end() const;

    private:
        class Compiler;
        std::unordered_map<std::string, std::shared_ptr<Block>> _hash;
    };

}
