#pragma once

#include <set>
#include <functional>
#include <css/selector.hpp>

#include "StyleHash.h"

namespace gpugraph
{

    class Node;

    class StyleSelector
    {
    public:
        struct PathItem 
        {
            using PseudoSelector = std::function<bool(Node const&)>;
            css::selector::combinator combinator;
            StyleHash style_hash;
            std::vector<PseudoSelector> pseudo_selectors;
        };
        using Path = std::vector <PathItem>;

        StyleSelector(css::selector const&);
        css::selector const& definition() const;

        // https://www.w3.org/TR/2011/REC-CSS2-20110607/cascade.html#specificity
        std::uint64_t specificity() const;
        Path const& path() const;
        bool is_applicable_to(Node const&) const;

    private:
        css::selector _definition;
        Path _path;
        std::uint64_t _specificity = 0;
    };

}
