#!/bin/sh

rm -Rf ../*.tar.gz
rm -Rf ../RMixtCompIO.Rcheck
make clean
make updateLib
make all
