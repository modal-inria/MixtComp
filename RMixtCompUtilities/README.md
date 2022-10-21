# RMixtCompUtilities

*RMixtCompUtilities* contains graphical, setter, summary functions for facilitating the analysis of MixtComp's output. It is used by [RMixtComp](../RMixtComp).

You can download it from CRAN in an R console:

```
install.packages("RMixtCompUtilities", repos = "https://cran.rstudio.com")
```

## Requirements

### R

R must be installed, along with the following packages from the CRAN: *plotly*, *scales*. Several packages are used for testing but are not required for installing RMixtCompUtilities: *testthat* is used to perform unit testing in R, *xml2* for xml report, *roxygen2* is required for generating the documentation and *devtools* (with *roxygen2*) is required to load all functions of $RMixtComp$ on the CI server.

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
    Rscript -e "install.packages(c(\"plotly\", \"scales\"), repos = \"https://cran.rstudio.com\")"
    # OPTIONAL: for testing purpose
    Rscript -e "install.packages(c(\"testthat\", \"RInside\", \"xml2\", \"devtools\", \"roxygen2\", \"RMixtCompIO\", \"Rmixmod\"), repos = \"https://cran.rstudio.com\")"
    ```
    from R:

    ```
    install.packages(c("plotly", "scales"), repos = "https://cran.rstudio.com")
    # OPTIONAL: for dev/testing purpose
    install.packages(c("testthat", "RInside", "xml2", "devtools", "roxygen2", "RMixtCompIO", "Rmixmod"), repos = "https://cran.rstudio.com")
    ```

### Windows

Compiling R packages requires [Rtools](https://cran.r-project.org/bin/windows/Rtools/).


## Build and install

Run in the current location (./RMixtCompUtilities/ folder):

```
R CMD INSTALL .
```

## Testing procedures

### Test the R package

Tests are implemented in the `tests/testthat` folder of RMixtCompUtilities. They require the *testthat* package.

You can run tests via R:

```
library(RMixtCompUtilities)
library(testthat)
library(devtools)
load_all()
test_dir("tests/testthat/")
```
or
```
library(methods);
library(devtools);
options(testthat.output_file = "RMCUttest.xml");
test(".", reporter = JunitReporter)
```
but it requires the *xml2* package and generates a xml report file named *RMCUttest.xml*.

Via shell:
```
Rscript -e 'library(methods); library(devtools); options(testthat.output_file = "RMCUttest.xml"); test(".", reporter = JunitReporter)'
```

In Rstudio, if packages *devtools* and *testthat* are installed, you can run the testing procedure by pressing `CRTL + SHIFT + T` or tests are run during a R CMD check `CRTL + SHIFT + E`.
