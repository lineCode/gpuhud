#pragma once

#include <css/selector.hpp>

namespace gpugraph
{

    struct Style::Selector
    {
    public:
        using PathItem = std::tuple<css::selector::combinator, std::set<std::string>>;
        using Path = std::vector <PathItem>;

        Selector(css::selector const&);

        std::uint64_t specificity() const;

        Path const& path() const;

    private:
        // this is the compiled resp. reversed path of the definition
        Path _path;
        // https://www.w3.org/TR/2011/REC-CSS2-20110607/cascade.html#specificity (we ignore spec. "a")
        std::uint64_t _specificity = 0;
    };

    struct Style::Block
    {
        Selector selector;
        Styling styling;
    };

}
