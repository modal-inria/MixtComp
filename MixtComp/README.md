
# Compile MixtComp

MixtComp uses cmake to generate makefile.

## Linux

Boost, Eigen and Nlopt libraries are required for MixtComp to compile.

You can install them with the following command on Linux:
```
sudo apt install cmake libboost-all-dev libnlopt-dev libeigen3-dev libxml2-dev
```

## OSX

The developper tools must be installed, by typing `xcode-select --install` in a terminal. This will provide a C++ compiler and a build toolchain.

The additional libraries can be installed using Homebrew.
```
brew install cmake boost nlopt eigen
```

## Windows
To install the necessary tools, user needs to install [cygwin](https://www.cygwin.com/).
gcc, cmake, boost and eigen must be installed through the cygwin packages installer.

### Nlopt

Nlopt is not available with Cygwin and need to be installed by hand. Go to [nlopt website](https://nlopt.readthedocs.io/en/latest/#download-and-installation) and download the tar.gz archive.
Extract the content of the archive (e.g. with [7-zip](https://www.7-zip.org/)) and put all the contents (api, auglag, ...) in the *nlopt* folder.
Then download the [CMakeLists file](http://ab-initio.mit.edu/nlopt/CMakeLists.txt) and the [config.cmake.h.in file](http://ab-initio.mit.edu/nlopt/config.cmake.h.in) from this [page](https://nlopt.readthedocs.io/en/latest/NLopt_on_Windows/).

nlopt will then be built automatically when MixtComp or JSonMixtComp is built.

## Compilation & tests

Run `./builDebug.sh` (resp. `./builRelease.sh`) to compile MixtComp in Debug (resp. Release) mode.
Run `./utestDebug.sh` (resp. `utestRelease.sh`) to run unit tests in Debug (resp. Release) mode.
To clean the debug and release folders, run `./clean.sh`.

