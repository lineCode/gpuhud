#pragma once

#include <unordered_map>
#include "Node.h"

namespace gpugraph 
{

    struct PseudoCssSelector
    {
        struct PseudoCssSelector(std::string const&);
    };

    class NodeHash
    {
    public:
        using Set = std::set<std::shared_ptr<Node>>;

        void insert(std::shared_ptr<Node>, bool recursive=true);
        void remove(std::shared_ptr<Node> const&, bool recursive=true);

        void hash(std::string& css_selector);

    private:
        std::unordered_map<std::string, Set> _id_hash;
        std::unordered_map<std::string, Set> _class_hash;
    };

}