# Build Instructions:

* unzip boost to some directory
* bootstrap boost
* locate the bjam-binary
* `cd` into `tools/bcp`
* run bjam inside `tools/bcp`
* run `bcp boost/spirit/x3.hpp out_spirit`
* copy or move `out_spirit` to `extern/boost`