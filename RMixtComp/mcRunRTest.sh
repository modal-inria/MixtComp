make cleanRMCbase
make all
Rscript -e 'library(methods); library(testthat); library(devtools); suppressWarnings(library(later)); suppressWarnings(library(crosstalk));library(shiny);library(plotly);
load_all(); options(testthat.output_file = "RMCtest.xml"); test_dir("tests/testthat/", reporter = JunitReporter)'
