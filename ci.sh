#!/bin/sh

# Script to be launched on a continuous integration machine

cd MixtComp
./clean.sh
./buildDebug.sh
./valgrindRun.sh
cd ../JMixtComp
./clean.sh
./buildDebug.sh
./valgrindRun.sh
cd ../RMixtCompIO
make clean
make updateLib
make all
Rscript -e 'library(methods); library(devtools); options(testthat.output_file = "RMCIOtest.xml"); test(".", reporter = JunitReporter)'
cd ../RMixtCompUtilities
R CMD INSTALL  --no-multiarch --with-keep.source .
Rscript -e 'library(methods); library(devtools); options(testthat.output_file = "RMCUttest.xml"); test(".", reporter = JunitReporter)'
cd ../RMixtComp
R CMD INSTALL  --no-multiarch --with-keep.source .
Rscript -e 'library(methods); library(devtools); options(testthat.output_file = "RMCtest.xml"); test(".", reporter = JunitReporter)'
cd ../RJMixtComp
./update.sh
R CMD INSTALL  --no-multiarch --with-keep.source .
Rscript -e 'library(methods); library(devtools); options(testthat.output_file = "RJMCtest.xml"); test(".", reporter = JunitReporter)'
# cd ../RMixtCompHier
# R CMD INSTALL  --no-multiarch --with-keep.source .
# Rscript -e 'library(methods); library(devtools); options(testthat.output_file = "RMCHiertest.xml"); test(".", reporter = JunitReporter)'
