#include "NodeTree.h"

#include <iostream>
#include <fstream>

#include <gpugraph/XmlLoader.h>

namespace gpuhud
{

    NodeTree::NodeTree(std::string const& filename)
    {
        std::ifstream stream(filename);
        auto tree = gpugraph::XmlLoader::load(stream);
        if (!tree)
        {
            std::cerr << "[hud] failed to load \"" << filename << "\"" << std::endl;
            return;
        }
        _root_node = tree;
    }

}