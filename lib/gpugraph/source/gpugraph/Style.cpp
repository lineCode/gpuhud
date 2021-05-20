#include "StyleCompiler.h"

#include <css/parser.hpp>
#include "StyleSelector.h"

namespace gpugraph
{

    void Style::compile(std::string const& source)
    {
        Compiler compiler(*this);
        css::parser().parse(source, compiler);
    }

    Style::Styling Style::make_styling(Node const&) const
    {
        throw "";
    }

}
