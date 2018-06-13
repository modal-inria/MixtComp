# Code organization

* **MixtComp** MixtComp C++ library
* **JsonMixtComp** Use MixtComp with json files input and output (for customers)
* **RMixtComp** R package linking MixtComp with Rcpp (internal use)
* **RJsonMixtComp** R package using JsonMixtComp (for customers)
* **libraries** external libraries and cmake scripts

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


# Install RMixtComp

## R

R must be installed, along with the following packages from the CRAN: *Rcpp*, *jsonlite*, *scales*, *plotly*. Two packages are used for testing but are not required for installing RMixtComp: *testthat* is used to perform unit testing in R and *RInside* is used for testing procedure in C++.

*testthat* and *xml2* must be installed.

Those command lines can perform the installation automatically in the ~/R directory
add the following line to .bashrc file: export R_LIBS="~/R"
```
mkdir ~/R
Rscript -e "install.packages(c(\"Rcpp\", \"scales\", \"jsonlite\", \"plotly\"), repos = \"https://cran.univ-paris1.fr/\")"
```
If you want a minimal installation of RMixtComp, you can edit both the DESCRIPTION and NAMESPACE files to keep only the *Rcpp* dependency.

## Windows

Compiling R packages requires [Rtools](https://cran.r-project.org/bin/windows/Rtools/).

## Build and install

After the initial `git --clone`, or after any update via `git pull` for example, the code must be rebuilt. To handle the various dependencies correctly, it is necessary to rebuild MixtComp by calling `./mcUpdate.sh` which is in the same directory as this present document. The script will clean just enough location to ensure a proper build without taking too long to complete. If compilation errors occur, for example cache errors, try running `./mcCleanUpdate.sh` which rebuild everything and take longer.

## Test in RStudio

Launch R by loading the RStudio project. Press `CRTL + SHIFT + B` to build and load the package.
If packages *devtools* and *testthat* are installed, you can run the testing procedure by pressing  `CRTL + SHIFT + T`.

Tests are implemented in the tests/testthat folder of RMixtComp.

For your own testing procedure, you can use the data and descriptor files provided in the package, their paths can be obtained using:
```
pathToData <- system.file("extdata", "data.csv", package = "RMixtComp")
pathToDescriptor <- system.file("extdata", "descriptor.csv", package = "RMixtComp")
```

Or you can generate your own data using functions:

- dataGenerator
- categoricalGenerator
- gaussianGenerator
- poissonGenerator
- ordinalGenerator
- rankGenerator
- functionalGenerator

These functions require a boolean vector indicating if the corresponding indivual is missing and a list of parameters. See the following functions for how to have an example of parameter object for each type of data:

- categoricalParam
- gaussianParam
- poissonParam
- ordinalParam
- rankParam
- functionalParam1sub
