# Compile MixtComp

MixtComp uses cmake to generate a makefile. It also requires the following external libraries:
- Boost
- Eigen

Note that if you are installing RMixtComp from CRAN, Boost and Eigen are provided via dependencies from CRAN, so you only have to install CMake manually.

## Linux

On Debian / Ubuntu, you can install all the dependencies using:

```
sudo apt install cmake libboost-all-dev libeigen3-dev libxml2-dev
```

## macOS

The developper tools must be installed, by typing `xcode-select --install` in a terminal. This will provide a C++ compiler and a build toolchain.

We recommand using Homebrew to manage dependencies. Installation is easy, follow instructions at https://brew.sh/

The additional libraries can then be installed using:

```
brew install cmake boost eigen
```

## Windows

To install the necessary tools, user needs to install [cygwin](https://www.cygwin.com/). gcc, cmake, boost and eigen must be installed through the cygwin packages installer.

## Compilation & tests

Run `./builDebug.sh` (resp. `./builRelease.sh`) to compile MixtComp in Debug (resp. Release) mode.
Run `./utestDebug.sh` (resp. `./utestRelease.sh`) to run unit tests in Debug (resp. Release) mode.
To clean the debug and release folders, run `./clean.sh`.


# Compilation FLAGS

Those compilation flags activate various behaviours in MixtComp.

- `MC_OUT`: general outputs
- `MC_DEBUG`: legacy macro which enables a lot of output, only used to keep a record of debug outputs for later uses
- `MC_PROGRESS`: activates real time writing of a `progress` file in the current working directory. Used mainly in MASSICCC.
- `MC_VERBOSE`: information on timer
- `MC_TIMERVERBOSE`: timer information on each iteration, with estimated remaining time

Note that `MC_DETERMINISTIC` is not a compilation flag but an environment variable.
