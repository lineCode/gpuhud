#include "Style.h"
#include "StyleSelector.h"

#include <iostream>

#include "log.h"

namespace gpugraph
{

    Style::Selector::Selector(css::selector const& definition)
    {
        std::uint64_t sa = 0 /* hash */;
        std::uint64_t sb = 0 /* attribte & class */;
        std::uint64_t sc = 0 /*type + pseudo (excl. ":not")*/;
        for (auto& item : definition.path)
        {
            std::set<std::string> combined;
            if (!item.type_selector.empty() && item.type_selector != "*")
            {
                combined.insert(item.type_selector);
                ++sc;
            }
            for (auto& class_ : item.class_selectors)
            {
                combined.insert("." + class_);
                ++sb;
            }
            for (auto& id : item.hash_selectors)
            {
                combined.insert("#" + id);
                ++sa;
            }
            _path.push_back({ item.combinator, std::move(combined) });
        }
        _specificity = sa << 32 | sb << 16 | sc << 0;
        log_debug("created selector (sa=" << sa << ", sb=" << sb << ", sc=" << sc << "): " << definition);
    }

    std::uint64_t Style::Selector::specificity() const
    {
        return _specificity;
    }

    Style::Selector::Path const& Style::Selector::path() const
    {
        return _path;
    }

}
