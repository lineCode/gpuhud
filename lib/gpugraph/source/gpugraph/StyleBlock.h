#pragma once

#include "Style.h"
#include "StyleBlock.h"

namespace gpugraph
{

    struct Style::Block
    {
        Block(std::shared_ptr<Selector>, Styling);

        bool can_be_linked_to(Node const&);
        bool is_applicable_to(Node const&);

        std::shared_ptr<Selector> selector;
        Styling styling;
    };

}
