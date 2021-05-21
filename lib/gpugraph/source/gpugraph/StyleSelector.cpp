#include "Style.h"
#include "StyleSelector.h"

#include <iostream>

#include "log.h"
#include "Node.h"

namespace gpugraph
{

    Style::Selector::Selector(css::selector const& definition)
        : _definition(definition)
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

    bool Style::Selector::is_selecting(Node const& node) const
    {
        Node const* node_iterator = &node;
        auto it = _path.rbegin();
        auto end = _path.rend();
        //
        // combinator for first entry in reverse order is always none
        css::selector::combinator combinator = css::selector::combinator::none;
        //
        // loop until reaching the root node
        while (node_iterator)
        {
            bool fulfilled = it->style_hash.is_subset_of(node_iterator->style_hash());
            fulfilled = fulfilled && std::all_of(it->pseudo_selectors.begin(), it->pseudo_selectors.end(), [node_iterator](auto& pseudo) {
                return pseudo(*node_iterator);
            });
            if (!fulfilled)
            {
                if (combinator == css::selector::combinator::greater)
                    return false;
                if (combinator != css::selector::combinator::none)
                {
                    log_error("tilde- and plus-combinator aren't supported yet");
                    return false;
                }
                // else:
                // check if parent can fulfill
            }
            else if (++it == end)
            {
                return true;
            }
            node_iterator = node_iterator->parent();
        }
        return it == end;
    }

    css::selector const& Style::Selector::definition() const
    {
        return _definition;
    }


}
