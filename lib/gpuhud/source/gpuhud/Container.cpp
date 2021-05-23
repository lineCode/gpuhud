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

#include "Container.h"

namespace gpuhud
{
    Container::Container()
        : Widget()
        , _content_node(root_node())
    {
    }

    void Container::add(std::shared_ptr<Widget> widget)
    {
        _content_node->add(widget->root_node());
        _children.push_back(widget);
    }

    void Container::remove(std::shared_ptr<Widget> widget)
    {
        _children.erase(std::remove_if(_children.begin(), _children.end(), [&](auto& child) {
            if (child == widget)
            {
                _content_node->remove(child->root_node());
                return true;
            }
            return false;
        }), _children.end());
    }

    void Container::clear()
    {
        for (auto& widget : _children)
            _content_node->remove(widget->root_node());
        _children.clear();
    }

    void Container::set_content_node(std::shared_ptr<gpugraph::Node> node)
    {
        _content_node = node;
    }

}
