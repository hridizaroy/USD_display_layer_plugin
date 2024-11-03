# USD Display Layer Plugin
A plugin for [Pixar's USD](https://github.com/PixarAnimationStudios/USD) (Universal Scene Description). This project defines a custom USD schema with a C++ API and a usdview plugin for a Display Layer. This will help users organize and control visibility of prims easily.

## Table of Contents

- [USD Plugins](#usd-plugins)
- [Dependencies](#dependencies)
- [Building](#building)
- [Running](#running)
- [Attributions](#attributions)

## USD Plugins

**USD Display Layer Plugin** provides the following USD plugins:
- [displayLayer](./src/displayLayer): A schema library defining a **DisplayLayer** prim type.
- [usdviewDisplayLayer](./src/usdviewDisplayLayer): An usdview plugin providing a menu command to open a utility UI for managing the Display Layer.

## Dependencies

The following dependencies are required:
- C++ compiler
- [CMake](https://cmake.org/documentation/) (Tested with CMake 3.27.1)
- [USD](https://github.com/pixaranimationstudios/USD) (24.08)
- [Boost](https://boost.org) and [Intel TBB](https://www.threadingbuildingblocks.org/) (USD dependencies)
- [Python](https://www.python.org/)

## Building
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

Replace `"~/USD/"` with your USD installation directory, and `"~/displayLayerPlugin/"` with where you wish to install this plugin.

## Running

To register the plugin as part of the USD runtime, the following environment variables will need
to be defined (instructions for Linux):
| Environment Variable  | Value(s)                                                                                 |
| --------------------- | ---------------------------------------------------------------------------------------- |
| `PYTHONPATH`          | `${DISPLAYLAYERPLUGIN_INSTALL_ROOT}/lib/python`                                           |
| `PXR_PLUGINPATH_NAME` | `${DISPLAYLAYERPLUGIN_INSTALL_ROOT}/lib/usd`<br/>`${DISPLAYLAYERPLUGIN_INSTALL_ROOT}/plugin/usd` |
| `LD_LIBRARY_PATH`     | `${DISPLAYLAYERPLUGIN_INSTALL_ROOT}/lib`                                                  |

## Attributions
This project uses https://github.com/wetadigital/USDPluginExamples as a template for creating the build structure.