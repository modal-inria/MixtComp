make cleanRMCbase
make all
Rscript -e 'library(methods); library(testthat); library(RMixtComp); options(testthat.output_file = "RMCtest.xml"); test_dir("tests/testthat/", reporter = JunitReporter)'
