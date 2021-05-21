#include <gpuhud/Window.h>
#include <gpuhud/NodeTree.h>

int main(int argc, char* argv[])
{
    gpuhud::Window window;
    window.load_style("style.css");
    window.add(std::make_shared<gpuhud::NodeTree>("tree.xml"));
    window.loop();
}
