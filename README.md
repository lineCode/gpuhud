# GpuHud
C++ - Gui based on CSS, Skia & ImPlot

Design Goals:
* Easy to Learn & Rapid Development: Use existing, well known CSS-specification and provide only lightweight additional API's.
* Performant: Hardware-Acceleration & Tiled Rendering
* Combine Exciting Projects: [Skia-Python](https://pypi.org/project/skia-python/), [ImPlot](https://github.com/epezent/implot) ...
* Look Pretty: Use Skia's facilities for drawing smooth 2D-objects etc.
* Python-Bindings

## Build Instructions:
Skia needs to be compiled first.
That's the same setup as seen as in [Skia Python](https://github.com/kyamagu/skia-python). You can find their instructions [here](https://kyamagu.github.io/skia-python/install.html). I had to run `gclient` like [here](https://commondatastorage.googleapis.com/chrome-infra-docs/flat/depot_tools/docs/html/depot_tools_tutorial.html).
