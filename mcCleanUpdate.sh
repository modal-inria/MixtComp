#!/bin/sh

# Script to be launched on a machine for a user that will run RMixtComp

cd RMixtComp
git pull
make clean
make updatelib
make all
