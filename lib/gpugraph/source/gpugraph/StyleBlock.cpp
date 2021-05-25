/***************************************************************************//*/
  Copyright (c) 2021 Martin Rudoff

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software. 

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
/******************************************************************************/

#include "StyleBlock.h"
#include "StyleSelector.h"
#include "Node.h"

namespace gpugraph
{

    StyleBlock::StyleBlock(std::shared_ptr<StyleSelector> selector, std::vector<StyleRule> rules)
        : _selector(std::move(selector))
        , _rules(std::move(rules))
    {
    }

    bool StyleBlock::is_linkable_to(Node const& node) const
    {
        return _selector->path().back().style_hash.is_subset_of(node.style_hash());
    }

    bool StyleBlock::is_applicable_to(Node const& node) const
    {
        return _selector->is_applicable_to(node);
    }

    std::shared_ptr<StyleSelector> const& StyleBlock::selector() const
    {
        return _selector;
    }

    std::vector<StyleRule> const& StyleBlock::rules() const
    {
        return _rules;
    }

    void StyleBlock::apply_to(ComputedStyleSet const& parent, ComputedStyleSet& set)
    {
        for (auto& rule : _rules)
            rule(parent, set);
    }

}
