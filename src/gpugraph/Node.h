#pragma once

#include <memory>
#include <vector>

#include "types.h"

namespace gpugraph
{

    class Intermediate;

    class Node
    {
    public:
        Node();

        void add(std::shared_ptr<Node>);
        void remove(std::shared_ptr<Node>);

        void set_use_intermediate(bool);
        bool use_intermediate() const;

    private:
        vec4 _margin;
        vec4 _border_width;
        vec4 _padding;

        Node* _parent;
        std::vector<std::shared_ptr<Node>> _children;
        std::shared_ptr<Intermediate> _intermediate;

        bool _use_intermediate = false;
    };

}