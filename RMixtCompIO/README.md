# RMixtCompIO

RMixtCompIO is the minimal R interface of the MixtComp C++ library. It is recommended to use [RMixtComp](../RMixtComp) which is more versatile and convenient to use. The version number of RMixtCompIO follows that of MixtComp C++ library. 

You can download it from CRAN in an R console:

```
install.packages("RMixtCompIO", repos = "https://cran.rstudio.com")
```

## Requirements

### R

R must be installed, along with the following packages from the CRAN: *RcppEigen*, *BH*, *doParallel*, *foreach*. Several packages are used for testing but are not required for installing RMixtComp: *testthat* is used to perform unit testing in R, *xml2* for xml report, *RInside* is used for testing procedure in C++ using R command, *roxygen2* is required for generating the documentation and *devtools* (with *roxygen2*) is required to load all functions of RMixtComp on the CI server.

1. Install R from terminal or download it from [R-project](https://www.r-project.org/):
    ```
    sudo apt-get install r-base
    ```

2. Install required R packages

    Those command lines can perform the installation automatically in the *~/R* directory.
    Add the following line to .bashrc file: export R_LIBS="~/R". This is not mandatory, if you have already defined a personal library.

    Packages are required from Ubuntu repositories: 
    ```sudo apt install libssl-dev libcurl4-openssl-dev libxml2-dev```
    
    Install required R packages from terminal:
    ```
    mkdir ~/R
    Rscript -e "install.packages(c(\"BH\", \"RcppEigen\", \"doParallel\", \"foreach\"), repos = \"https://cran.rstudio.com\")"
    # OPTIONAL: for testing purpose
    Rscript -e "install.packages(c(\"testthat\", \"RInside\", \"xml2\", \"devtools\", \"roxygen2\"), repos = \"https://cran.rstudio.com\")"
    ```
    from R:
    ```
    install.packages(c("BH", "RcppEigen", "doParallel", "foreach"), repos = "https://cran.rstudio.com")
    # OPTIONAL: for testing purpose
    install.packages(c("testthat", "RInside", "xml2", "devtools", "roxygen2"), repos = "https://cran.rstudio.com")
    ```

### Windows

Compiling R packages requires [Rtools](https://cran.r-project.org/bin/windows/Rtools/).


## Build and install

Run in the current location (./RMixtCompIO/ folder):

```
make updateLib
make all
```

The target *updateLib* copies the MixtComp library C++ folder from [MixtComp/src/lib](../MixtComp/src/lib) folder to [src/lib folder](src/lib).

## Testing procedures

### Test the R package

Tests are implemented in the `tests/testthat` folder of RMixtComp. They require the *testthat* package.

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
