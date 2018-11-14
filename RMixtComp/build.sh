#!/bin/sh

rm -Rf ../*.tar.gz
rm -Rf ../RMixtComp.Rcheck
make clean
make updateLib
make all
