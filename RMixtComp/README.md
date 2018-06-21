
# Install RMixtComp


## Requirements

### R

R must be installed, along with the following packages from the CRAN: *Rcpp*. Two packages are used for testing but are not required for installing RMixtComp: *testthat* is used to perform unit testing in R, *xml2* for xml report and *RInside* is used for testing procedure in C++.

Those command lines can perform the installation automatically in the ~/R directory
add the following line to .bashrc file: export R_LIBS="~/R"
```
mkdir ~/R
Rscript -e "install.packages(c(\"Rcpp\"), repos = \"https://cran.univ-paris1.fr/\")"
# OPTIONAL
Rscript -e "install.packages(c(\"testthat\", \"RInside\", \"xml2\"), repos = \"https://cran.univ-paris1.fr/\")"
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
test_dir("tests/testthat/")
```

You can run tests via shell (it requires the *xml2* package):
```
./mcRunRTest.sh
```
It generates a xml report file named *RMCtest.xml*.


In Rstudio, if packages *devtools* and *testthat* are installed, you can run the testing procedure by pressing  `CRTL + SHIFT + T`.


### Test the C++ code from RMixtComp

It requires *RIinside* package. Tests are in the *src/test* folder. You can compile and run it with:
```
./testBuildRun.sh
```

### Writing new tests
For your own testing procedure, you can use the data and descriptor files provided in the package, their paths can be obtained using:
```
pathToData <- system.file("extdata", "data.csv", package = "RMixtComp")
pathToDescriptor1 <- system.file("extdata", "descriptorSupervised.csv", package = "RMixtComp")
pathToDescriptor2 <- system.file("extdata", "descriptorUnsupervised.csv", package = "RMixtComp")
```

Or you can generate your own data using function (from files R/GENDATA_dataGenerator.R):

- dataGenerator

See the following functions for how to have an example of parameter object for each type of data (from files R/TEST_*.R):

- categoricalParam1
- categoricalParam2
- categoricalParamRandom
- gaussianParam
- poissonParam
- poissonParamRandom
- weibullParam
- ordinalParam1
- ordinalParam2
- ordinalParamRandom
- rankParam
- functionalParam1sub
- functionalParam2sub
- functionalInterPolyParam
- functionalSharedAlphaInterPolyParam
- functionalSharedAlphaParam1sub
- functionalSharedAlphaParam2sub
