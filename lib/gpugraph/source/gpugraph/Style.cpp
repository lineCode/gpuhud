#include "StyleCompiler.h"

#include <css/parser.hpp>
#include "StyleSelector.h"

namespace gpugraph
{

    void Style::compile(std::string const& source)
    {
        Compiler compiler(*this);

    }

    Style::Styling Style::make_styling(Node const&) const
    {
        throw "";
    }

}