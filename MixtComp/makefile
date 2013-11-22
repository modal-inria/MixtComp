#-----------------------------------------------------------------------
#     Copyright (C) 2004-2007  Serge Iovleff
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU Lesser General Public License as
#    published by the Free Software Foundation; either version 2 of the
#    License, or (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU Lesser General Public License for more details.
#
#    You should have received a copy of the GNU Lesser General Public
#    License along with this program; if not, write to the
#    Free Software Foundation, Inc.,
#    59 Temple Place,
#    Suite 330,
#    Boston, MA 02111-1307
#    USA
#
#    Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
#
#-----------------------------------------------------------------------
#
# Purpose:  Main makefile of MixtComp
# Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
#
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
# Macros and definitions are in MAKEVARS, they can be overloaded by up call
ifeq ($(wildcard ./makevars.local),) 
RELATIVEMAKEVARS = ./makevars
else 
RELATIVEMAKEVARS = ./makevars.local
endif

MAKEVARS = $(realpath ./makevars)

include $(MAKEVARS)

#-----------------------------------------------------------------------
# paths

# build paths
BIN_RELEASE_DIR = $(realpath ./bin/release)
BIN_DEBUG_DIR   = $(realpath ./bin/debug)

# sources paths
SRC_FRAMEWORK_DIR         := $(realpath ./src/framework/)
SRC_DATAHANDLING_DIR      := $(realpath ./src/dataHandling/)
SRC_MIXTURE_INTERFACE_DIR := $(realpath ./src/mixtureInterface/)
SRC_MIXTURES_DIR          := $(realpath ./src/mixtures/)
SRC_TESTS_DIR             := $(realpath ./tests/)

# examples path
EXAMPLES_DIR              := $(realpath ./examples/)

#STK build path
LIB_STK := $(realpath ./src/stkpp/lib/libSTKpp.a)

#Rcpp build path
LIB_RCPP := $(realpath ./src/Rcpp/lib/libRcpp.a)

#-----------------------------------------------------------------------
# export variables, build paths and compile flags, to share them with sub-makefiles
export MAKEVARS
export LIB_STK
export LIB_RCPP

#-----------------------------------------------------------------------
# build rules

# phony prerequisites are executed unconditionally
.PHONY: release debug clean examples

release:
	$(MAKE) -C $(SRC_FRAMEWORK_DIR) BIN_DIR="$(BIN_RELEASE_DIR)" CXXFLAGS=$(CXXFLAGS_RELEASE)
	$(MAKE) -C $(SRC_DATAHANDLING_DIR) BIN_DIR="$(BIN_RELEASE_DIR)" CXXFLAGS=$(CXXFLAGS_RELEASE)
	$(MAKE) -C $(SRC_MIXTURE_INTERFACE_DIR) BIN_DIR="$(BIN_RELEASE_DIR)" CXXFLAGS=$(CXXFLAGS_RELEASE)
	$(MAKE) -C $(SRC_MIXTURES_DIR) BIN_DIR="$(BIN_RELEASE_DIR)" CXXFLAGS=$(CXXFLAGS_RELEASE)
	$(MAKE) -C $(SRC_TESTS_DIR) BIN_DIR="$(BIN_RELEASE_DIR)" CXXFLAGS=$(CXXFLAGS_RELEASE)

debug:
	$(MAKE) -C $(SRC_FRAMEWORK_DIR) BIN_DIR="$(BIN_DEBUG_DIR)" CXXFLAGS=$(CXXFLAGS_DEBUG)
	$(MAKE) -C $(SRC_DATAHANDLING_DIR) BIN_DIR="$(BIN_DEBUG_DIR)" CXXFLAGS=$(CXXFLAGS_DEBUG)
	$(MAKE) -C $(SRC_MIXTURE_INTERFACE_DIR) BIN_DIR="$(BIN_DEBUG_DIR)" CXXFLAGS=$(CXXFLAGS_DEBUG)
	$(MAKE) -C $(SRC_MIXTURES_DIR) BIN_DIR="$(BIN_DEBUG_DIR)" CXXFLAGS=$(CXXFLAGS_DEBUG)
	$(MAKE) -C $(SRC_TESTS_DIR) BIN_DIR="$(BIN_DEBUG_DIR)" CXXFLAGS=$(CXXFLAGS_DEBUG)

examples:
	$(MAKE) -C $(EXAMPLES_DIR) BIN_DIR="$(BIN_DEBUG_DIR)" CXXFLAGS=$(CXXFLAGS_DEBUG)
#	$(MAKE) -C $(EXAMPLES_DIR) BIN_DIR="$(BIN_RELEASE_DIR)" CXXFLAGS=$(CXXFLAGS_RELEASE)

clean:
	$(RM) ./bin/debug/*
	$(RM) ./bin/release/*