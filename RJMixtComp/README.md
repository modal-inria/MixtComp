# Install RJMixtComp

This package uses JMixtComp. It requires packages *jsonlite* for managing json files, *roxygen2* for generating the documentation and *testthat* for unit testing.

```
install.packages("jsonlite", repos = "https://cran.rstudio.com")
install.packages(c("roxygen2", "jsonlite"), repos = "https://cran.rstudio.com")
```

1. Run `./update.sh` for compiling JMixtComp and copying required files.
2. Install the package: `R CMD INSTALL --no-multiarch --with-keep.source .` or `CTRL + Shift + B` in RStudio.
