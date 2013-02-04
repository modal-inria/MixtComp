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
#    Contact : Serge.Iovleff@stkpp.org
#
#-----------------------------------------------------------------------
#
# Purpose:  Main makefile of the Project STKpp.
# Author:   Serge Iovleff, serge.iovleff@stkpp.org
#
#-----------------------------------------------------------------------
# Macros and definitions
include ./makevars

#-----------------------------------------------------------------------
# wrapper for cmake
#-----------------------------------------------------------------------
.PHONY: cmake cclean cdoc ctest
cmake: ./build/Makefile
	cd build/; cmake ../ -DCMAKE_VERBOSE_MAKEFILE=true
	$(MAKE) -C build  VERBOSE=1

./build/Makefile:
	cd build >/dev/null 2>&1 && cmake .. -DCMAKE_VERBOSE_MAKEFILE=true

cclean:
	@- (cd build >/dev/null 2>&1 && cmake .. >/dev/null 2>&1)
	@- $(MAKE) --silent -C build clean || true
	@- $(RM) ./build/Makefile
	@- $(RM) ./build/src
	@- $(RM) ./build/test
	@- $(RM) -rf ./build/CMake*
	@- $(RM) ./build/cmake.*
	@- $(RM) ./build/*.cmake
	@- $(RM) ./build/*.txt
	@- $(RM) ./build/Doxyfile
	
cdoc:
	cd build/; $(MAKE) doc

ctest:
	cd build; $(MAKE) test

#-----------------------------------------------------------------------
# handmade make
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
# all compiles the objects and create the library, no tests are build
.PHONY: all debug
all: obj lib

#-----------------------------------------------------------------------
# main Paths
PROJECTS_DIR = ./projects/
GUI_DIR = ./gui/
BIN_DIR = ./bin/
LIB_DIR = ./lib/

TUTO_DIR = ./doc/tutorial/
MIXBIN_DIR = ./tutorials/MixBin/src

#-----------------------------------------------------------------------
# specific projects paths
STKERNEL_DIR = $(PROJECTS_DIR)STKernel/
ARRAYS_DIR = $(PROJECTS_DIR)Arrays/
SDK_DIR = $(PROJECTS_DIR)Sdk/
DMANAGER_DIR = $(PROJECTS_DIR)DManager/
ALGEBRA_DIR = $(PROJECTS_DIR)Algebra/
STATISTIK_DIR = $(PROJECTS_DIR)STatistiK/
AAMODELS_DIR = $(PROJECTS_DIR)AAModels/
REGRESS_DIR = $(PROJECTS_DIR)Regress/
REDUCT_DIR = $(PROJECTS_DIR)Reduct/
MODEL_DIR = $(PROJECTS_DIR)StatModels/

SRC_STKERNEL_DIR = $(STKERNEL_DIR)src/
SRC_ARRAYS_DIR = $(ARRAYS_DIR)src/
SRC_SDK_DIR = $(SDK_DIR)src/
SRC_DMANAGER_DIR = $(DMANAGER_DIR)src/
SRC_ALGEBRA_DIR = $(ALGEBRA_DIR)src/
SRC_STATISTIK_DIR = $(STATISTIK_DIR)src/
SRC_AAMODELS_DIR = $(AAMODELS_DIR)src/
SRC_REGRESS_DIR = $(REGRESS_DIR)src/
SRC_REDUCT_DIR = $(REDUCT_DIR)src/
SRC_MODEL_DIR = $(MODEL_DIR)src/

TEST_DIR = tests/
TEST_ARRAYS_DIR = $(TEST_DIR)Arrays
TEST_ALGEBRA_DIR = $(TEST_DIR)Algebra
TEST_DMANAGER_DIR = $(TEST_DIR)DManager


#-----------------------------------------------------------------------
# projects binaries
STKERNEL_OBJS = $(patsubst $(SRC_STKERNEL_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_STKERNEL_DIR)/*.cpp))
ARRAYS_OBJS = $(patsubst $(SRC_ARRAYS_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_ARRAYS_DIR)/*.cpp))
SDK_OBJS = $(patsubst $(SRC_SDK_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_SDK_DIR)/*.cpp))
ALGEBRA_OBJS = $(patsubst $(SRC_ALGEBRA_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_ALGEBRA_DIR)/*.cpp))
DMANAGER_OBJS = $(patsubst $(SRC_DMANAGER_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_DMANAGER_DIR)/*.cpp))
STATISTIK_OBJS = $(patsubst $(SRC_STATISTIK_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_STATISTIK_DIR)/*.cpp))
REGRESS_OBJS =$(patsubst $(SRC_REGRESS_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_REGRESS_DIR)/*.cpp))
AAMODELS_OBJS = $(patsubst $(SRC_AAMODELS_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_AAMODELS_DIR)/*.cpp))
REDUCT_OBJS = $(patsubst $(SRC_REDUCT_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_REDUCT_DIR)/*.cpp))
MODEL_OBJS = $(patsubst $(SRC_MODEL_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_MODEL_DIR)/*.cpp))

# all binaries
CPP_OBJS = \
  $(STKERNEL_OBJS) \
  $(ARRAYS_OBJS) \
  $(SDK_OBJS) \
  $(ALGEBRA_OBJS) \
  $(DMANAGER_OBJS) \
  $(STATISTIK_OBJS) \
  $(AAMODELS_OBJS) \
  $(REGRESS_OBJS) \
  $(REDUCT_OBJS) \
  $(MODEL_OBJS)

# library name
STK_LIB = $(LIB_DIR)$(LIB_STKPP_A)

#-----------------------------------------------------------------------
# create the library
$(STK_LIB): $(CPP_OBJS)
	$(AR) $(ARFLAGS) $@ $(CPP_OBJS)

#-----------------------------------------------------------------------
# obj compiles the objects in all projects
.PHONY: obj
obj: stkernel sdk dmanager arrays aamodels regress reduct algebra statistik \
     statmodel

#-----------------------------------------------------------------------
# create the library
.PHONY: lib
lib: obj $(STK_LIB)

#-----------------------------------------------------------------------
# rules if we want to compile only one sub-project (assuming nothing
# else is modified)
.PHONY: stkernel sdk dmanager arrays aamodels regress reduct algebra statistik \
        statmodel
stkernel:
	$(MAKE) -C $(SRC_STKERNEL_DIR)
arrays:
	$(MAKE) -C $(SRC_ARRAYS_DIR)
sdk:
	$(MAKE) -C $(SRC_SDK_DIR)
dmanager:
	$(MAKE) -C $(SRC_DMANAGER_DIR)
algebra:
	$(MAKE) -C $(SRC_ALGEBRA_DIR)
statistik:
	$(MAKE) -C $(SRC_STATISTIK_DIR)
aamodels:
	$(MAKE) -C $(SRC_AAMODELS_DIR)
regress:
	$(MAKE) -C $(SRC_REGRESS_DIR)
reduct:
	$(MAKE) -C $(SRC_REDUCT_DIR)
statmodel:
	$(MAKE) -C $(SRC_MODEL_DIR)

#-----------------------------------------------------------------------
# rule if we want to compile test programs
.PHONY: tests testsCsv testArrays testAlgebra
tests: obj lib
	cd $(TEST_DIR); $(MAKE) all

testsArrays: obj lib
	cd $(TEST_ARRAYS_DIR); $(MAKE) all

testsAlgebra: obj lib
	cd $(TEST_ALGEBRA_DIR); $(MAKE) all

testsDManager: obj lib
	cd $(TEST_DMANAGER_DIR); $(MAKE) all

testsCsv: obj lib
	cd $(TEST_DMANAGER_DIR); $(MAKE) testCsv

testsPages: obj lib
	cd $(TEST_DMANAGER_DIR); $(MAKE) testPages

alltests: tests testsArrays testsAlgebra testsDManager

#-----------------------------------------------------------------------
# rule if we want to compile tutorial programs
.PHONY: tuto mixbin
tuto: obj lib
	cd $(TUTO_DIR); $(MAKE) all

mixbin:
	cd $(MIXBIN_DIR); $(MAKE) all

#-----------------------------------------------------------------------
# rules for cleaning objects
.PHONY: cleanlib cleanobj cleantests cleantuto clean cleanmixbin
clean: cleanlib cleanobj cleantests cleantuto

cleanlib:
	$(RM) $(LIB_DIR)$(LIB_STKPP_A)
cleanobj:
	$(RM) $(BIN_DIR)*.o
cleantests:
	$(RM) $(BIN_DIR)test*
cleantuto:
	$(RM) $(BIN_DIR)tuto*
cleanmixbin:
	cd $(MIXBIN_DIR); $(MAKE) clean


#-----------------------------------------------------------------------
# gui project
.PHONY: qt guimodel gui
qt:
	cd ./gui/qt/; qmake; $(MAKE)

guimodel:
	cd ./gui/model/src; $(MAKE) all

gui: guimodel qt
