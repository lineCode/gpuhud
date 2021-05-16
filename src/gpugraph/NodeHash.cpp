#include "NodeHash.h"

#include "string_utils.h"

namespace gpugraph
{

    PseudoCssSelector::PseudoCssSelector(std::string const& expression)
    {
        auto parts = tokenize(expression, "\\s+");
    }

    void NodeHash::insert(std::shared_ptr<Node> node, bool recursive)
    {
        _id_hash[node->id()].insert(node);
        for (auto& class_ : node->class_set())
            _class_hash[class_].insert(node);
        if (recursive) for (auto& child : *node) insert(child);
    }

    void NodeHash::remove(std::shared_ptr<Node> const& node, bool recursive)
    {
        _id_hash.erase(node->id());
        for (auto& class_ : node->class_set())
        {
            _class_hash[class_].erase(node);
            if (_class_hash[class_].size() == 0)
                _class_hash.erase(class_);
        }
        if (recursive) for (auto& child : *node) remove(child);
    }

}
