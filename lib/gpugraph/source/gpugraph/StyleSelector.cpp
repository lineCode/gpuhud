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
            StyleHash style_hash;
            if (!item.type_selector.empty() && item.type_selector != "*")
            {
                style_hash.insert_type(item.type_selector);
                ++sc;
            }
            for (auto& class_ : item.class_selectors)
            {
                style_hash.insert_class(class_);
                ++sb;
            }
            for (auto& id : item.hash_selectors)
            {
                style_hash.insert_id(id);
                ++sa;
            }
            _path.push_back(PathItem{ item.combinator, std::move(style_hash) });
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
