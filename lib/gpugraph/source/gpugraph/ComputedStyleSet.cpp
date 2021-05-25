#include "ComputedStyleSet.h"

namespace gpugraph
{
    ComputedStyleSet const ComputedStyleSet::Initial = ComputedStyleSet({
        {static_cast<std::uint8_t>(StyleIdentifier::Display), std::make_shared<ComputedStyle::Display>(css::display::inline_)},
        {static_cast<std::uint8_t>(StyleIdentifier::Position), std::make_shared<ComputedStyle::Position>(css::position::static_)},
        {static_cast<std::uint8_t>(StyleIdentifier::Left), std::make_shared<ComputedStyle::Left>(css::auto_())},
        {static_cast<std::uint8_t>(StyleIdentifier::Top), std::make_shared<ComputedStyle::Left>(css::auto_())},
        {static_cast<std::uint8_t>(StyleIdentifier::Right), std::make_shared<ComputedStyle::Left>(css::auto_())},
        {static_cast<std::uint8_t>(StyleIdentifier::Bottom), std::make_shared<ComputedStyle::Left>(css::auto_())},
        {static_cast<std::uint8_t>(StyleIdentifier::Width), std::make_shared<ComputedStyle::Left>(css::auto_())},
        {static_cast<std::uint8_t>(StyleIdentifier::Height), std::make_shared<ComputedStyle::Left>(css::auto_())},
        });
}
