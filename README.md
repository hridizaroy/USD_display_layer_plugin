Example snippet for building the plugins on Linux:
```bash
mkdir build
cd build
cmake \
  -DUSD_ROOT="~/USD/" \
  -DCMAKE_INSTALL_PREFIX="~/displayLayerPlugin/" \
  ..
cmake --build  . -- VERBOSE=1 -j8 all test install
```