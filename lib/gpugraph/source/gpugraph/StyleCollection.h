#pragma once

#include <functional>
#include <set>
#include <unordered_map>
#include <vector>

namespace gpugraph
{

    class Node;
    struct StyleBlock;
    using Styling = std::vector<std::shared_ptr<StyleBlock>>;

    class StyleCollection
    {
    public:
        void compile(std::string const&);
        Styling extract_linkable_styling_for(Node const&) const;

        std::unordered_map<std::string, std::shared_ptr<StyleBlock>>::const_iterator begin() const;
        std::unordered_map<std::string, std::shared_ptr<StyleBlock>>::const_iterator end() const;

    private:
        std::unordered_map<std::string, std::set<std::shared_ptr<StyleBlock>>> _hash;
    };

}
