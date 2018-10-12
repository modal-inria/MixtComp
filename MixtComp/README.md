# Compile MixtComp

MixtComp uses cmake to generate a makefile. It also requires the following external libraries:
- Boost
- Eigen
- Nlopt

## Linux

On Debian / Ubuntu, you can install all the dependencies using:

```
sudo apt install cmake libboost-all-dev libnlopt-dev libeigen3-dev libxml2-dev
```

## macOS

The developper tools must be installed, by typing `xcode-select --install` in a terminal. This will provide a C++ compiler and a build toolchain.

We recommand using Homebrew to manage dependencies. Installation is easy, follow instructions at https://brew.sh/

The additional libraries can then be installed using:

```
brew install cmake boost nlopt eigen
```

## Windows

To install the necessary tools, user needs to install [cygwin](https://www.cygwin.com/). gcc, cmake, boost and eigen must be installed through the cygwin packages installer.

### Nlopt

Nlopt is not available with Cygwin and need to be installed by hand. Go to [nlopt website](https://nlopt.readthedocs.io/en/latest/#download-and-installation) and download the tar.gz archive.

Extract the content of the archive (e.g. with [7-zip](https://www.7-zip.org/)) and put all the contents (api, auglag, ...) in the *nlopt* folder.
Then download the [CMakeLists file](http://ab-initio.mit.edu/nlopt/CMakeLists.txt) and the [config.cmake.h.in file](http://ab-initio.mit.edu/nlopt/config.cmake.h.in) from this [page](https://nlopt.readthedocs.io/en/latest/NLopt_on_Windows/).

nlopt will then be built automatically when MixtComp or JSonMixtComp is built.

## Compilation & tests

Run `./builDebug.sh` (resp. `./builRelease.sh`) to compile MixtComp in Debug (resp. Release) mode.
Run `./utestDebug.sh` (resp. `utestRelease.sh`) to run unit tests in Debug (resp. Release) mode.
To clean the debug and release folders, run `./clean.sh`.

# Compilation FLAGS

Those compilation flags activate various behaviours in MixtComp.

- `MC_DEBUG`: legacy macro which enables a lot of output
- `MC_PROGRESS`: activates real time writing of a `progress` file in the current working directory. Used mainly in MASSICCC.
- `MC_VERBOSE`: information on timer
- `MC_TIMERVERBOSE`: more information on timer, including estimated remaining time
