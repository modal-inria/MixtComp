make all
Rscript -e 'library(testthat); library(RMixtComp); options(testthat.output_file = "RMCtest.xml"); test_dir("tests/testthat/", reporter = JunitReporter)'
