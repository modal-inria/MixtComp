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
# Purpose:  Main makefile of the Project STKpp.
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

MAKEVARS = $(realpath $(RELATIVEMAKEVARS))
export RELATIVEMAKEVARS
export MAKEVARS

ifeq ($(MAKEVARS),)
	include $(RELATIVEMAKEVARS)
else
	include $(MAKEVARS)
endif

#-----------------------------------------------------------------------
# handmade make for building the library
#-----------------------------------------------------------------------
# main Paths
PROJECTS_DIR = ./projects/
BIN_DIR = ./bin/
LIB_DIR = ./lib/

#-----------------------------------------------------------------------
# library name
STK_LIB = $(LIB_DIR)$(LIB_STKPP_A)

#-----------------------------------------------------------------------
# projects paths
STKERNEL_DIR  = $(PROJECTS_DIR)STKernel/
ARRAYS_DIR    = $(PROJECTS_DIR)Arrays/
SDK_DIR       = $(PROJECTS_DIR)Sdk/
DMANAGER_DIR  = $(PROJECTS_DIR)DManager/
ALGEBRA_DIR   = $(PROJECTS_DIR)Algebra/
ANALYSIS_DIR  = $(PROJECTS_DIR)Analysis/
STATISTIK_DIR = $(PROJECTS_DIR)STatistiK/
STATMODELS_DIR= $(PROJECTS_DIR)StatModels/
CLUSTERING_DIR= $(PROJECTS_DIR)Clustering/
AAMODELS_DIR  = $(PROJECTS_DIR)AAModels/
REGRESS_DIR   = $(PROJECTS_DIR)Regress/
REDUCT_DIR    = $(PROJECTS_DIR)Reduct/

SRC_STKERNEL_DIR  = $(STKERNEL_DIR)src/
SRC_ARRAYS_DIR    = $(ARRAYS_DIR)src/
SRC_SDK_DIR       = $(SDK_DIR)src/
SRC_DMANAGER_DIR  = $(DMANAGER_DIR)src/
SRC_ALGEBRA_DIR   = $(ALGEBRA_DIR)src/
SRC_ANALYSIS_DIR  = $(ANALYSIS_DIR)src/
SRC_STATISTIK_DIR = $(STATISTIK_DIR)src/
SRC_STATMODELS_DIR= $(STATMODELS_DIR)src/
SRC_CLUSTERING_DIR= $(CLUSTERING_DIR)src/
SRC_REGRESS_DIR   = $(REGRESS_DIR)src/
SRC_REDUCT_DIR    = $(REDUCT_DIR)src/
SRC_AAMODELS_DIR  = $(AAMODELS_DIR)src/

#-----------------------------------------------------------------------
# projects binaries
STKERNEL_OBJS = $(patsubst $(SRC_STKERNEL_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_STKERNEL_DIR)/*.cpp))
ARRAYS_OBJS   = $(patsubst $(SRC_ARRAYS_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_ARRAYS_DIR)/*.cpp))
SDK_OBJS      = $(patsubst $(SRC_SDK_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_SDK_DIR)/*.cpp))
ALGEBRA_OBJS  = $(patsubst $(SRC_ALGEBRA_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_ALGEBRA_DIR)/*.cpp))
ANALYSIS_OBJS = $(patsubst $(SRC_ANALYSIS_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_ANALYSIS_DIR)/*.cpp))
DMANAGER_OBJS = $(patsubst $(SRC_DMANAGER_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_DMANAGER_DIR)/*.cpp))
STATISTIK_OBJS= $(patsubst $(SRC_STATISTIK_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_STATISTIK_DIR)/*.cpp))
STATMODELS_OBJS= $(patsubst $(SRC_STATMODELS_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_STATMODELS_DIR)/*.cpp))
CLUSTERING_OBJS= $(patsubst $(SRC_CLUSTERING_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_CLUSTERING_DIR)/*.cpp))
REGRESS_OBJS  = $(patsubst $(SRC_REGRESS_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_REGRESS_DIR)/*.cpp))
REDUCT_OBJS   = $(patsubst $(SRC_REDUCT_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_REDUCT_DIR)/*.cpp))
AAMODELS_OBJS = $(patsubst $(SRC_AAMODELS_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_AAMODELS_DIR)/*.cpp))

#-----------------------------------------------------------------------
# all binaries
CPP_OBJS = \
  $(STKERNEL_OBJS) \
  $(ARRAYS_OBJS) \
  $(SDK_OBJS) \
  $(DMANAGER_OBJS) \
  $(ALGEBRA_OBJS) \
  $(ANALYSIS_OBJS) \
  $(STATISTIK_OBJS) \
  $(STATMODELS_OBJS) \
  $(CLUSTERING_OBJS) \
  $(REGRESS_OBJS) \
  $(REDUCT_OBJS) \
  $(AAMODELS_OBJS)

#-----------------------------------------------------------------------
# all compiles the objects and create the library, no tests are build
.PHONY: all lib obj
all: lib

# Create the library. Build the objects of each projects and then add them
lib: obj $(STK_LIB)

# obj compiles the objects in all projects
obj: stkernel sdk dmanager arrays algebra analysis statistik statmodel clustering \
     aamodels regress reduct

#-----------------------------------------------------------------------
# rules for compiling each projects
.PHONY: stkernel arrays sdk dmanager algebra analysis statistik statmodel clustering \
        aamodels regress reduct
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
analysis:
	$(MAKE) -C $(SRC_ANALYSIS_DIR)
statistik:
	$(MAKE) -C $(SRC_STATISTIK_DIR)
statmodel:
	$(MAKE) -C $(SRC_STATMODELS_DIR)
clustering:
	$(MAKE) -C $(SRC_CLUSTERING_DIR)
aamodels:
	$(MAKE) -C $(SRC_AAMODELS_DIR)
regress:
	$(MAKE) -C $(SRC_REGRESS_DIR)
reduct:
	$(MAKE) -C $(SRC_REDUCT_DIR)

#-----------------------------------------------------------------------
# create the library adding the objects
$(STK_LIB): $(CPP_OBJS)
	$(AR) $(ARFLAGS) $@ $(CPP_OBJS)

#-----------------------------------------------------------------------
# rules for cleaning objects
.PHONY: clean_lib clean_obj clean clean_stkernel clean_arrays clean_sdk \
  clean_dmanager clean_algebra clean_analysis clean_statistik clean_statmodel \
  clean_clustering  clean_aamodels clean_regress clean_reduct

clean: clean_lib clean_obj

clean_lib:
	$(RM) $(LIB_DIR)$(LIB_STKPP_A)

clean_obj: \
  clean_stkernel \
  clean_arrays \
  clean_sdk \
  clean_dmanager \
  clean_algebra \
  clean_analysis \
  clean_statistik \
  clean_statmodel \
  clean_clustering \
  clean_aamodels \
  clean_regress \
  clean_reduct

clean_stkernel:
	$(RM) $(STKERNEL_OBJS)
clean_arrays:
	$(RM) $(ARRAYS_OBJS)
clean_sdk:
	$(RM) $(SDK_OBJS)
clean_dmanager:
	$(RM) $(DMANAGER_OBJS)
clean_algebra:
	$(RM) $(ALGEBRA_OBJS)
clean_analysis:
	$(RM) $(ANALYSIS_OBJS)
clean_statistik:
	$(RM) $(STATISTIK_OBJS)
clean_statmodel:
	$(RM) $(STATMODELS_OBJS)
clean_clustering:
	$(RM) $(CLUSTERING_OBJS)
clean_aamodels:
	$(RM) $(AAMODELS_OBJS)
clean_regress:
	$(RM) $(REGRESS_OBJS)
clean_reduct:
	$(RM) $(REDUCT_OBJS)

#-----------------------------------------------------------------------
# wrapper for cmake
#-----------------------------------------------------------------------
.PHONY: cmake cclean cdoc ctest

cmake_debug: #./build/Makefile
	cd build/; cmake ../ -DCMAKE_VERBOSE_MAKEFILE=true
	$(MAKE) -C build  VERBOSE=1

cmake_release: #./build/Makefile
	cd build/; cmake ../ -DCMAKE_VERBOSE_MAKEFILE=true -DCMAKE_BUILD_TYPE=Release
	$(MAKE) -C build  VERBOSE=1

cmake_debug_0: #./build/Makefile
	cd build/; cmake ../ -DCMAKE_VERBOSE_MAKEFILE=true -DZERO_BASED_ARRAYS=on
	$(MAKE) -C build  VERBOSE=1

cmake_release_0: #./build/Makefile
	cd build/; cmake ../ -DCMAKE_VERBOSE_MAKEFILE=true -DZERO_BASED_ARRAYS=on -DCMAKE_BUILD_TYPE=Release
	$(MAKE) -C build  VERBOSE=1

#./build/Makefile:
#	cd build/ >/dev/null 2>&1 && cmake .. -DCMAKE_VERBOSE_MAKEFILE=true

cmake_clean:
	@- cd build/; $(MAKE) clean  VERBOSE=1
	@- $(RM) ./build/Makefile
	@- $(RM) ./build/src
	@- $(RM) ./build/test
	@- $(RM) ./build/CMake*
	@- $(RM) ./build/cmake.*
	@- $(RM) ./build/*.cmake
	@- $(RM) ./build/*.txt
	@- $(RM) ./build/Doxyfile
	
cmake_doc:
	cd build/; $(MAKE) doc

cmake_test:
	cd build/; $(MAKE) test

#-----------------------------------------------------------------------
# Tests
#-----------------------------------------------------------------------
# paths of the tests programs
TESTS_DIR = tests/
TESTS_STKERNEL_DIR = $(TESTS_DIR)STKernel/
TESTS_ARRAYS_DIR = $(TESTS_DIR)Arrays/
TESTS_AAMODELS_DIR = $(TESTS_DIR)AAModels/
TESTS_ALGEBRA_DIR = $(TESTS_DIR)Algebra/
TESTS_ANALYSIS_DIR = $(TESTS_DIR)Analysis/
TESTS_DMANAGER_DIR = $(TESTS_DIR)DManager/
TESTS_STATMODELS_DIR = $(TESTS_DIR)StatModels/
TESTS_STATISTIK_DIR = $(TESTS_DIR)STatistiK/
TESTS_REGRESS_DIR = $(TESTS_DIR)Regress/
TESTS_CLUSTERING_DIR = $(TESTS_DIR)Clustering/

#-----------------------------------------------------------------------
# rule if we want to compile test programs
.PHONY: testsAAModels testsCsv testArrays testAlgebra testDManager testsStatModels \
testRegress testsSTatistiK testsPages alltests testClustering clean_tests

testsSTKernel: obj lib
	cd $(TESTS_STKERNEL_DIR); $(MAKE) all
testsArrays: obj lib
	cd $(TESTS_ARRAYS_DIR); $(MAKE) all
testsAnalysis: obj lib
	cd $(TESTS_ANALYSIS_DIR); $(MAKE) all
testsAlgebra: obj lib
	cd $(TESTS_ALGEBRA_DIR); $(MAKE) all
testsRegress: obj lib
	cd $(TESTS_REGRESS_DIR); $(MAKE) all
testsDManager: obj lib
	cd $(TESTS_DMANAGER_DIR); $(MAKE) all
testsSTatistiK: obj lib
	cd $(TESTS_STATISTIK_DIR); $(MAKE) all
testsStatModels: obj lib
	cd $(TESTS_STATMODELS_DIR); $(MAKE) all
testsAAModels: obj lib
	cd $(TESTS_AAMODELS_DIR); $(MAKE) all
testsClustering: obj lib
	cd $(TESTS_CLUSTERING_DIR); $(MAKE) all
# shortcut for Csv and Pages tests
testsCsv: obj lib
	cd $(TESTS_DMANAGER_DIR); $(MAKE) testCsv
testsPages: obj lib
	cd $(TESTS_DMANAGER_DIR); $(MAKE) testPages

alltests: testsSTKernel testsArrays testsAnalysis testsAlgebra testsDManager \
testsStatModels testsRegress testsAAModels testsSTatistiK testsClustering

clean_tests:
	$(RM) $(BIN_DIR)test*

#-----------------------------------------------------------------------
# Tutorials
#-----------------------------------------------------------------------
# rule if we want to compile tutorial and programs
PROGRAMS_DIR = ./programs/
TUTORIALS_DIR = ./doc/tutorial/

MIXBIN_DIR = $(PROGRAMS_DIR)MixBin/src
CONTIGUITY_DIR = $(PROGRAMS_DIR)Contiguity/src

.PHONY: tuto mixbin contiguity
tuto: obj lib
	cd $(TUTORIALS_DIR); $(MAKE) all

.PHONY: tuto mixbin
mixbin:
	cd $(MIXBIN_DIR); $(MAKE) all

contiguity:
	cd $(CONTIGUITY_DIR); $(MAKE) all

cleantuto:
	$(RM) $(BIN_DIR)tuto*
cleanmixbin:
	cd $(MIXBIN_DIR); $(MAKE) clean
cleancontiguity:
	cd $(CONTIGUITY_DIR); $(MAKE) clean


#-----------------------------------------------------------------------
#-----------------------------------------------------------------------
# gui project (to remove)
.PHONY: qt guimodel gui
qt:
	cd ./gui/qt/; qmake; $(MAKE)

guimodel:
	cd ./gui/model/src; $(MAKE) all

gui: guimodel qt
