make cleanRMCbase
make all
Rscript -e 'library(methods); library(testthat); library(devtools); load_all(); options(testthat.output_file = "RMCtest.xml"); test_dir("tests/testthat/", reporter = JunitReporter)'
