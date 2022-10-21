
# Install RMixtComp

*RMixtComp* is the main R package for using the MixtComp C++ library. It loads [RMixtCompIO](../RMixtCompIO) and [RMixtCompUtilities](../RMixtCompUtilities) that contain the MixtComp C++ library and utility functions.

*RMixtComp* is available as an [Rstudio](https://www.rstudio.com/products/rstudio/download/#download) project ([RMixtComp.Rproj](RMixtComp.Rproj)) to facilitate development and testing.

You can download it from CRAN in an R console:

``` r
install.packages("RMixtComp", repos = "https://cran.rstudio.com")
```

## Requirements

### R

R must be installed, along with the following packages from the CRAN: *RcppEigen*, *BH*, *plotly*, *scales*, *doParallel*, *foreach* and packages from these directories: [RMixtCompIO](../RMixtCompIO) and [RMixtCompUtilities](../RMixtCompUtilities). Several packages are used for testing but are not required for installing RMixtComp: *testthat* is used to perform unit testing in R, *xml2* for xml report, *RInside* is used for testing procedure in C++ using R command, *roxygen2* is required for generating the documentation, *knitr* for generating vignettes and *devtools* (with *roxygen2*) is required to load all functions of *RMixtComp* on the CI server.

1. Install R from terminal or download it from [R-project](https://www.r-project.org/):

    ```bash
    sudo apt install r-base
    ```

2. Install required R packages

    Packages are required from Ubuntu repositories:

    ```bash
    sudo apt install libssl-dev libcurl4-openssl-dev libxml2-dev
    ```

    Install required R packages from terminal:

    ```bash
    Rscript -e "install.packages(c(\"plotly\", \"scales\", \"BH\", \"RcppEigen\", \"doParallel\", \"foreach\"), repos = \"https://cran.rstudio.com\")"
    # OPTIONAL: for testing purpose
    Rscript -e "install.packages(c(\"testthat\", \"RInside\", \"xml2\", \"devtools\", \"roxygen2\", \"Rmixmod\", \"knitr\", \"ClusVis\"), repos = \"https://cran.rstudio.com\")"
    ```

    from R:

    ```r
    install.packages(c("plotly", "scales", "BH", "RcppEigen", "doParallel", "foreach"), repos = "https://cran.rstudio.com")
    # OPTIONAL: for testing purpose
    install.packages(c("testthat", "RInside", "xml2", "devtools", "roxygen2", "Rmixmod", "knitr", "ClusVis"), repos = "https://cran.rstudio.com")
    ```

3. Install [RMixtCompIO](../RMixtCompIO)

    ```bash
    cd ../RMixtCompIO
    ./build.sh
    ```

4. Install [RMixtCompUtilities](../RMixtCompUtilities)

    ```bash
    cd ../RMixtCompUtilities
    R CMD INSTALL  --no-multiarch --with-keep.source .
    ```

### Windows

Compiling R packages requires [Rtools](https://cran.r-project.org/bin/windows/Rtools/).

## Build and install

Run in the current location (./RMixtComp/ folder):

```bash
R CMD INSTALL  --no-multiarch --with-keep.source .
```

## Testing procedures

### Test the R package

Tests are implemented in the `tests/testthat` folder of RMixtComp. They require the *testthat* package.

You can run tests via R:

```r
library(RMixtComp)
library(testthat)
library(devtools)
load_all()
test_dir("tests/testthat/")
```

or

```r
library(methods);
library(devtools);
options(testthat.output_file = "RMCtest.xml");
test(".", reporter = JunitReporter)
```

but it requires the *xml2* package and generates a xml report file named *RMCtest.xml*.

Via shell:

```bash
Rscript -e 'library(methods); library(devtools); options(testthat.output_file = "RMCtest.xml"); test(".", reporter = JunitReporter)'
```

In Rstudio, if packages *devtools* and *testthat* are installed, you can run the testing procedure by pressing `CRTL + SHIFT + T` or tests are run during a R CMD check `CRTL + SHIFT + E`.

### Write new tests

For your own testing procedure, you can use the data and model provided in the package, you can load them in R:

```r
data(simData)
```

Or you can generate your own data using dataGeneratorNewIO function (from files [R/GENDATA_dataGeneratorNewIO.R](../RMixtCompIO/R/GENDATA_dataGeneratorNewIO.R)) for example.

See functions in files R/TEST_\*.R for creating a parameter object for each type of data. Have a look at [tests/testthat/test.run.R](tests/testthat/test.run.R) for writing a test with *testthat*.
