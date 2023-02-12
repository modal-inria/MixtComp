# Compile MixtComp

*MixtComp* uses cmake to generate a makefile. It also requires the following external libraries:

- [Boost](https://www.boost.org/)
- [Eigen](http://eigen.tuxfamily.org)

Note that if you are installing *RMixtComp* from CRAN, Boost and Eigen are provided via dependencies from CRAN.

## Linux

On Debian / Ubuntu, you can install all the dependencies using:

``` bash
sudo apt install cmake libboost-all-dev libeigen3-dev libxml2-dev lcov
```

## macOS

The developer tools must be installed, by typing `xcode-select --install` in a terminal. This will provide a C++ compiler and a build toolchain.

We recommand using Homebrew to manage dependencies. Installation is easy, follow instructions at <https://brew.sh/>

The additional libraries can then be installed using:

``` bash
brew install cmake boost eigen lcov
```

## Windows

To install the necessary tools, user needs to install [cygwin](https://www.cygwin.com/). gcc, cmake, boost and eigen must be installed through the cygwin packages installer. Note: WSL might be a better solution, but does it allows to compare with CRAN builds for example ?

## Compilation & tests

Run `./build.sh` to compile MixtComp in Debug mode. Run `./build.sh Release` or `./build.sh Coverage` to build in release mode or for code coverage. The build is performed in a build folder.
Run `./utest.sh` to run unit tests.
To clean the build folder, run `./clean.sh`.

### Compilation FLAGS

Those compilation flags activate various behaviours in MixtComp.

- `MC_OUT`: general outputs
- `MC_DEBUG`: legacy macro which enables a lot of output, only used to keep a record of debug outputs for later uses
- `MC_PROGRESS`: activates real time writing of a `progress` file in the current working directory. Used mainly in MASSICCC.
- `MC_VERBOSE`: information on timer
- `MC_TIMERVERBOSE`: timer information on each iteration, with estimated remaining time

Note that `MC_DETERMINISTIC` is not a compilation flag but an environment variable.
