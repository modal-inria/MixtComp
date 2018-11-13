#!/bin/sh

# Script to be launched on a continuous integration machine

cd MixtComp
./clean.sh
./buildDebug.sh
./valgrindRun.sh
cd ../RMixtComp
make clean
make updatelib
make all
Rscript -e 'library(methods); library(devtools); options(testthat.output_file = "RMCtest.xml"); test(".", reporter = JunitReporter)'
