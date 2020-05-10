# The TnT Engine

TnT is a 2d Game Engine written in C++. At the time of speaking the engine uses
features from the C++17 standard (as there is no C++ compiler that fully
supports C++20 features), but one of the goals of the engine is to use C++20
features wherever possible.

|Windows|Linux|Mac OS-X|
|:-----:|:---:|:------:|
|[![Build Status](https://dev.azure.com/terenst17/TnT/_apis/build/status/TerensTare.tnt?branchName=master&jobName=Build%20for%20&configuration=Build%20for%20windows)](https://dev.azure.com/terenst17/TnT/_build/latest?definitionId=4&branchName=master)|[![Build Status](https://dev.azure.com/terenst17/TnT/_apis/build/status/TerensTare.tnt?branchName=master&jobName=Build%20for%20&configuration=Build%20for%20linux)](https://dev.azure.com/terenst17/TnT/_build/latest?definitionId=4&branchName=master)|[![Build Status](https://dev.azure.com/terenst17/TnT/_apis/build/status/TerensTare.tnt?branchName=master&jobName=Build%20for%20&configuration=Build%20for%20mac)](https://dev.azure.com/terenst17/TnT/_build/latest?definitionId=4&branchName=master)|
|[![Windows](https://ci.appveyor.com/api/projects/status/a0rfndievu7neo1a?svg=true)](https://ci.appveyor.com/project/TerensTare/tnt)|

## Project Goals

The project aims to make game development easy enough for anybody that doesn't
know C++. Meanwhile the engine should be powerful enough for a C++ developer.

## Features

- Fast 2d rendering.
- Efficient Entity management.
- Fast and easy GUI library.
- Lightweight logging.
- Networking support.
- Data serialization (save into/load from file).
- Scripting support.

## Code quality

|Code checker|Badge|
|:----------:|:---:|
| CodeFactor | [![CodeFactor](https://www.codefactor.io/repository/github/terenstare/tnt/badge)](https://www.codefactor.io/repository/github/terenstare/tnt) |
| Codacy     | [![Codacy Badge](https://api.codacy.com/project/badge/Grade/3749bb4e09c74f6785177d318bb8ba15)](https://app.codacy.com/manual/terens.t17/tnt?utm_source=github.com&utm_medium=referral&utm_content=TerensTare/tnt&utm_campaign=Badge_Grade_Dashboard)
| Code Inspector| ![](https://www.code-inspector.com/project/7251/score/svg)  ![](https://www.code-inspector.com/project/7251/status/svg)

## Documentation

The documentation of the engine is written on the header files of the `include` directory. It is built by default by CMake. Note that for building the documentation, you need to have Doxygen installed (you can get it [here](http://www.doxygen.nl/manual/install.html)). The documentation will be built on `build/docs` folder. If you don't want to build the docs, please run `cmake -DTNT_BUILD_DOCS=OFF ..` instead of `cmake ..`. To read the documentation, please open `build/docs/html/index.html` on your browser.

## Getting started

For getting the binaries or building from source, please refer to [INSTALL.md](./INSTALL.md).

### Building the tests and samples

WIP

## Contributing

Please see [CONTRIBUTING.md](./CONTRIBUTING.md) to learn how you can contribute to this project.
