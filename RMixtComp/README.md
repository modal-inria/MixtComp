
# Install RMixtComp


## Requirements

### R

R must be installed, along with the following packages from the CRAN: *Rcpp*, *plotly*, *scales*. Several packages are used for testing but are not required for installing RMixtComp: *testthat* is used to perform unit testing in R, *xml2* for xml report, *RInside* is used for testing procedure in C++ using R command, *roxygen2* is required for generating the documentation and *devtools* (with *roxygen2*) is required to load all functions of RMixtComp on the CI server.

Those command lines can perform the installation automatically in the ~/R directory
add the following line to .bashrc file: export R_LIBS="~/R"
```
mkdir ~/R
Rscript -e "install.packages(c(\"Rcpp\", \"plotly\", \"scales\"), repos = \"https://cran.rstudio.com\")"
# OPTIONAL: for testing purpose
Rscript -e "install.packages(c(\"testthat\", \"RInside\", \"xml2\", \"devtools\", \"roxygen2\"), repos = \"https://cran.rstudio.com\")"
```

### Windows

Compiling R packages requires [Rtools](https://cran.r-project.org/bin/windows/Rtools/). 

**WARNING:** does not work for the moment. For windows, use RJsonMixtComp.



## Build and install

After the initial `git --clone`, or after any update via `git pull` for example, the code must be rebuilt. To handle the various dependencies correctly, it is necessary to rebuild MixtComp by calling `./mcUpdate.sh` which is in the same directory as this present document. The script will clean just enough location to ensure a proper build without taking too long to complete. If compilation errors occur, for example cache errors, try running `./mcCleanUpdate.sh` which rebuild everything and take longer.



## Testing procedures

### Test the R package
Tests are implemented in the `tests/testthat` folder of RMixtComp. They require the *testthat* package.

You can run tests via R:

```
library(RMixtComp)
library(testthat)
library(devtools)
load_all()
test_dir("tests/testthat/")
```

You can run tests via shell (it requires the *xml2* package):
```
./mcRunRTest.sh
```
It generates a xml report file named *RMCtest.xml*.


In Rstudio, if packages *devtools* and *testthat* are installed, you can run the testing procedure by pressing  `CRTL + SHIFT + T`.


### Test the C++ code from RMixtComp

It requires *RIinside* package. Tests are in the *src/test* folder. You can compile and run it from Eclipse.


### Writing new tests
For your own testing procedure, you can use the data and descriptor provided in the package, you can load them in R:
```
data(simData)
```

Or you can generate your own data using function (from files [R/GENDATA_dataGeneratorNewIO.R](R/GENDATA_dataGeneratorNewIO.R)): dataGeneratorNewIO.

See the functions in files R/TEST_*.R for how to have a parameter object for each type of data. Have a look at [tests/testthat/test.run.R](tests/testthat/test.run.R) for writing a test with testthat.

