make cleanRMCbase
make all
Rscript -e 'library(methods); library(devtools); options(testthat.output_file = "RMCtest.xml"); test(".", reporter = JunitReporter)'
