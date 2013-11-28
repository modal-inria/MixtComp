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
LIB_DIR = ./lib/

# sources paths
SRC_MIXTURES_DIR = $(realpath ./src/mixtures/)
SRC_TESTS_DIR    = $(realpath ./tests/)

# build path
LIB_STK = $(realpath $(STK_DIR)/lib/libSTKpp.a)
LIB_MC = $(realpath ./lib)/libMC.a

#-----------------------------------------------------------------------
# export variables, build paths and compile flags, to share them with sub-makefiles
export MAKEVARS
export LIB_STK

#-----------------------------------------------------------------------
# build rules

# phony prerequisites are executed unconditionally
.PHONY: lib release debug tests clean

release:
	$(MAKE) -C $(SRC_MIXTURES_DIR) BIN_DIR="$(BIN_RELEASE_DIR)" CXXFLAGS=$(CXXFLAGS_RELEASE)

debug:
	$(MAKE) -C $(SRC_MIXTURES_DIR) BIN_DIR="$(BIN_DEBUG_DIR)" CXXFLAGS=$(CXXFLAGS_DEBUG)

tests:
	$(MAKE) -C $(SRC_TESTS_DIR) BIN_DIR="$(BIN_RELEASE_DIR)" CXXFLAGS=$(CXXFLAGS_RELEASE)
	$(MAKE) -C $(SRC_TESTS_DIR) BIN_DIR="$(BIN_DEBUG_DIR)" CXXFLAGS=$(CXXFLAGS_DEBUG)

lib: release $(LIB_MC)

$(LIB_MC):
	$(AR) $(ARFLAGS) $@ $(wildcard $(BIN_RELEASE_DIR)/*.o)

clean:
	$(RM) ./bin/debug/*
	$(RM) ./bin/release/*
	$(RM) ./lib/*