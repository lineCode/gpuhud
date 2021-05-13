#pragma once

namespace gpugraph
{

    /// an intermediate rendering of a subtree
    class Intermediate
    {
    public:
    private:
        Intermediate* parent;
        std::shared_ptr<Intermediate> _children;
    };

}
