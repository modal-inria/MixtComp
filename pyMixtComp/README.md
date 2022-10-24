# pyMixtComp

*pyMixtCompBridge* is a C++ library containing the minimal interface of MixtComp for python.
The *pyMixtComp* python package is the equivalent of RMixtComp.

## Requirements

*pyMixtCompBridge* requires python3.8 and the *numpy* package. See [MixtComp's README](../MixtComp/README.md) for MixtComp's requirements.

``` bash
sudo apt install python3-pip
pip3 install numpy
```

It requires [Boost.Python](https://www.boost.org/doc/libs/1_72_0/libs/python/doc/html/index.html) library.

``` bash
brew install boost-python3
```

## Compile PyMixtComp

Run `./build.sh` to compile MixtComp in Debug mode. Run `./build.sh Release` for compiling in Release mode.
The resulting files are located in the build folder.
Run `./utest.sh` to run unit tests.
To clean the build folder, run `./clean.sh`.

## Python library

After compiling, *pyMixtCompBridge* library can be found at the following location: `build/lib/pyMixtCompBridge.so`.

## Python package

The python package is located in the [python](./python) folder.
See the [README](./python/README.md) to install the package.
