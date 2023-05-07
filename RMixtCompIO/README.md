# RMixtCompIO

*RMixtCompIO* is the minimal R interface of the MixtComp C++ library. It is recommended to use [RMixtComp](../RMixtComp) which is more versatile and convenient to use. The version number of *RMixtCompIO* follows that of MixtComp C++ library. 

You can download it from CRAN in an R console:

```
install.packages("RMixtCompIO", repos = "https://cran.rstudio.com")
```

## Requirements

### CppNumericalSolvers library

The C++ library *CppNumericalSolvers* is required in the *src* folder. It is available via a git submodule. Run in a terminal:

```
git submodule init
git submodule update
```

### R

R must be installed, along with the following packages from the CRAN: *RcppEigen*, *BH*, *doParallel*, *foreach*. Several packages are used for testing but are not required for installing RMixtComp: *testthat* is used to perform unit testing in R, *xml2* for xml report, *RInside* is used for testing procedure in C++ using R command, *roxygen2* is required for generating the documentation and *devtools* (with *roxygen2*) is required to load all functions of RMixtCompIO on the CI server.

1. Install R from terminal or download it from [R-project](https://www.r-project.org/):

    ```
    sudo apt install r-base
    ```

2. Install required R packages

    Packages are required from Ubuntu repositories: 
    
    ```
    sudo apt install libssl-dev libcurl4-openssl-dev libxml2-dev
    ```
    
    Install required R packages from terminal:
    
    ```
    Rscript -e "install.packages(c(\"BH\", \"RcppEigen\", \"doParallel\", \"foreach\"), repos = \"https://cran.rstudio.com\")"
    # OPTIONAL: for testing purpose
    Rscript -e "install.packages(c(\"testthat\", \"RInside\", \"xml2\", \"devtools\", \"roxygen2\", \"Rmixmod\", \"blockcluster\"), repos = \"https://cran.rstudio.com\")"
    ```
    from R:
    
    ```
    install.packages(c("BH", "RcppEigen", "doParallel", "foreach"), repos = "https://cran.rstudio.com")
    # OPTIONAL: for testing purpose
    install.packages(c("testthat", "RInside", "xml2", "devtools", "roxygen2", "Rmixmod", "blockcluster"), repos = "https://cran.rstudio.com")
    ```

### Windows

Compiling R packages requires [Rtools](https://cran.r-project.org/bin/windows/Rtools/).


## Install

Run in the current location (./RMixtCompIO/ folder):

```
make updateLib
make all
```

The target *updateLib* copies the MixtComp library C++ folder from [MixtComp/src/lib](../MixtComp/src/lib) folder to [src/lib folder](src/lib).

## Build

```bash
R CMD build RMixtCompIO
```

It generates a tar.gz file


## Check

```bash
R CMD check --as-cran RMixtCompIO
```

or you can build the package before

```bash
R CMD check --as-cran RMixtCompIO_*.tar.gz
```

### Windows

If you do not have windows, build the source and upload the tar.gz at <https://win-builder.r-project.org/upload.aspx>

### Custom linux configuration

To check on (almost) the same linux configuration as the CRAN, you can use the following procedure: <https://r-hub.github.io/containers/local.html>

1) Install [Docker](https://docs.docker.com/engine/install/ubuntu/)
1) Create a `check` folder and put the tar.gz package inside
1) Run

```bash
docker run -v `pwd`/check:/check ghcr.io/r-hub/containers/gcc13:latest r-check
```

See <https://r-hub.github.io/containers/containers.html> for available containers (`clang16`, `clang-asan`, ...)

## Testing procedures

### Test the R package

Tests are implemented in the `tests/testthat` folder of RMixtCompIO. They require the *testthat* package.

You can run tests via R:

```
library(RMixtCompIO)
library(testthat)
library(devtools)
load_all()
test_dir("tests/testthat/")
```
or 
```
library(methods); 
library(devtools); 
options(testthat.output_file = "RMCIOtest.xml"); 
test(".", reporter = JunitReporter)
```
but it requires the *xml2* package and generates a xml report file named *RMCIOtest.xml*.

Via shell:
```
Rscript -e 'library(methods); library(devtools); options(testthat.output_file = "RMCIOtest.xml"); test(".", reporter = JunitReporter)'
```

In Rstudio, if packages *devtools* and *testthat* are installed, you can run the testing procedure by pressing `CRTL + SHIFT + T` or tests are run during a R CMD check `CRTL + SHIFT + E`.



### Write new tests

For your own testing procedure, you can generate your own data using dataGeneratorNewIO function (from files [R/GENDATA_dataGeneratorNewIO.R](R/GENDATA_dataGeneratorNewIO.R)) for example.

See functions in files R/TEST_\*.R for creating a parameter object for each type of data. Have a look at [tests/testthat/test.run.R](tests/testthat/test.run.R) for writing a test with *testthat*.

### Test the C++ code from RMixtCompIO

It requires *RInside* package. Tests are in the *src/test* folder. You can compile and run it from Eclipse.
