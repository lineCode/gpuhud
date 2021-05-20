#include <gpuhud/Window.h>

int main(int argc, char* argv[])
{
    gpuhud::Window window;
    window.load_style("style.css");
    window.loop();
}
