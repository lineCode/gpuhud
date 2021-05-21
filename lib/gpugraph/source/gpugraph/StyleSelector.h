#pragma once

#include <set>
#include <css/selector.hpp>

#include "StyleHash.h"

namespace gpugraph
{

    struct Style::Selector
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

        Selector(css::selector const&);

        std::uint64_t specificity() const;

        Path const& path() const;

        bool is_selecting(Node const&) const;

        css::selector const& definition() const;

    private:
        css::selector _definition;
        // this is the compiled resp. reversed path of the definition
        Path _path;
        // https://www.w3.org/TR/2011/REC-CSS2-20110607/cascade.html#specificity (we ignore spec. "a")
        std::uint64_t _specificity = 0;
    };

}
