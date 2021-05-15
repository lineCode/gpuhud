#include "Loader.h"

#include <tinyxml2.h>

namespace gpugraph
{

    std::shared_ptr<Node> Loader::load(std::istream &stream)
    {
	    std::string buffer((std::istreambuf_iterator<char>(stream)),
        std::istreambuf_iterator<char>());

        tinyxml2::XMLDocument document;
        document.Parse(buffer.c_str(), buffer.size());

        return nullptr;
    }

}
