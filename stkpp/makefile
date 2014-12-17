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
# Macros and definitions are in makevars, they can be overloaded using makevars.local
# should contain at least the CXXFLAGS and the LIB_STKPP_A name.
ifeq ($(wildcard ./makevars.local),) 
	MAIN_MAKEVARS = makevars
else 
	MAIN_MAKEVARS = makevars.local
endif

include $(MAIN_MAKEVARS)

#-----------------------------------------------------------------------
# main Paths
MAIN_PROJECTS_DIR = projects/
MAIN_INCLUDE_DIR = include/
MAIN_LIB_DIR = lib/
MAIN_BIN_DIR = bin/
CMAKE_DIR = cmakedir/

#-----------------------------------------------------------------------
# library name
STKPP_LIB = $(MAIN_LIB_DIR)$(LIB_STKPP_A)

#-----------------------------------------------------------------------
# export common variables.
# check if we are using symoblic paths, in this case use absolute paths
# (may fail on windows platform), otherwise use relative paths
ifneq ($(patsubst $(MAIN_PROJECTS_DIR)STKernel/../../%,%,$(wildcard $(MAIN_PROJECTS_DIR)STKernel/../../*)),$(patsubst $(MAIN_PROJECTS_DIR)../%,%,$(wildcard $(MAIN_PROJECTS_DIR)../*)))
BIN_DIR = $(CURDIR)/$(MAIN_BIN_DIR)
PROJECTS_DIR = $(CURDIR)/$(MAIN_PROJECTS_DIR)
TEST_LIB_DIR = $(CURDIR)/$(MAIN_LIB_DIR)
TEST_BIN_DIR = $(CURDIR)/$(MAIN_BIN_DIR)
TEST_PROJECTS_DIR = $(CURDIR)/$(MAIN_PROJECTS_DIR)
TEST_INCLUDE_DIR = $(CURDIR)/$(MAIN_INCLUDE_DIR)
else
BIN_DIR = ../../../$(MAIN_BIN_DIR)
PROJECTS_DIR = ../..
TEST_LIB_DIR = ../../$(MAIN_LIB_DIR)
TEST_BIN_DIR = ../../$(MAIN_BIN_DIR)
TEST_PROJECTS_DIR = ../../$(MAIN_PROJECTS_DIR)
TEST_INCLUDE_DIR = ../../$(MAIN_INCLUDE_DIR)
endif

TEST_STKPP_LIB = $(TEST_LIB_DIR)$(LIB_STKPP_A)

export

#-----------------------------------------------------------------------
# all compiles the objects and create the library, no tests are build
.PHONY: all echo
all: lib
lib: $(STKPP_LIB)

# for debugging purpose
echo:
	@echo $(wildcard ./makevars.local)
	@echo $(MAIN_MAKEVARS)
	@echo $(LIB_STKPP_A)

#---------------------------------------------------------------------------
# Default: wrapper for cmake, if you don't want to use cmake, issue make all.
# See detailed INSTALL instructions
#---------------------------------------------------------------------------
.PHONY: cmake_debug cmake_release cmake_debug_0 cmake_release_0 cmake_clean cmake_doc cmake_test

cmake_debug_0: #./build/Makefile
	cd $(CMAKE_DIR); cmake ../ -DCMAKE_VERBOSE_MAKEFILE=true -DZERO_BASED_ARRAYS=on
	$(MAKE) -C cmakedir  VERBOSE=1

cmake_release_0: #./build/Makefile
	cd $(CMAKE_DIR); cmake ../ -DCMAKE_VERBOSE_MAKEFILE=true -DZERO_BASED_ARRAYS=on -DCMAKE_BUILD_TYPE=Release
	$(MAKE) -C cmakedir  VERBOSE=1

cmake_debug: #./build/Makefile
	cd $(CMAKE_DIR); cmake ../ -DCMAKE_VERBOSE_MAKEFILE=true -DZERO_BASED_ARRAYS=off
	$(MAKE) -C cmakedir  VERBOSE=1

cmake_release: #./build/Makefile
	cd $(CMAKE_DIR); cmake ../ -DCMAKE_VERBOSE_MAKEFILE=true  -DZERO_BASED_ARRAYS=off -DCMAKE_BUILD_TYPE=Release
	$(MAKE) -C cmakedir  VERBOSE=1

cmake_clean:
	@- cd $(CMAKE_DIR); $(MAKE) clean  VERBOSE=1
	@- $(RM) -rf ./cmakedir/*ake*  ./cmakedir/*est* ./cmakedir/projects ./cmakedir/D*
	
cmake_doc:
	cd $(CMAKE_DIR); $(MAKE) doc

cmake_test:
	cd $(CMAKE_DIR); $(MAKE) test

#-----------------------------------------------------------------------
# handmade make for building the library
#-----------------------------------------------------------------------
# projects paths
STKERNEL_DIR  = $(MAIN_PROJECTS_DIR)STKernel/
ARRAYS_DIR    = $(MAIN_PROJECTS_DIR)Arrays/
SDK_DIR       = $(MAIN_PROJECTS_DIR)Sdk/
DMANAGER_DIR  = $(MAIN_PROJECTS_DIR)DManager/
ALGEBRA_DIR   = $(MAIN_PROJECTS_DIR)Algebra/
ANALYSIS_DIR  = $(MAIN_PROJECTS_DIR)Analysis/
STATISTIK_DIR = $(MAIN_PROJECTS_DIR)STatistiK/
STATMODELS_DIR= $(MAIN_PROJECTS_DIR)StatModels/
CLUSTERING_DIR= $(MAIN_PROJECTS_DIR)Clustering/
AAMODELS_DIR  = $(MAIN_PROJECTS_DIR)AAModels/
REGRESS_DIR   = $(MAIN_PROJECTS_DIR)Regress/
REDUCT_DIR    = $(MAIN_PROJECTS_DIR)Reduct/

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
STKERNEL_OBJS = $(patsubst $(SRC_STKERNEL_DIR)%.cpp, $(MAIN_BIN_DIR)%.o, $(wildcard $(SRC_STKERNEL_DIR)*.cpp))
ARRAYS_OBJS   = $(patsubst $(SRC_ARRAYS_DIR)%.cpp, $(MAIN_BIN_DIR)%.o, $(wildcard $(SRC_ARRAYS_DIR)*.cpp))
SDK_OBJS      = $(patsubst $(SRC_SDK_DIR)%.cpp, $(MAIN_BIN_DIR)%.o, $(wildcard $(SRC_SDK_DIR)*.cpp))
ALGEBRA_OBJS  = $(patsubst $(SRC_ALGEBRA_DIR)%.cpp, $(MAIN_BIN_DIR)%.o, $(wildcard $(SRC_ALGEBRA_DIR)*.cpp))
ANALYSIS_OBJS = $(patsubst $(SRC_ANALYSIS_DIR)%.cpp, $(MAIN_BIN_DIR)%.o, $(wildcard $(SRC_ANALYSIS_DIR)*.cpp))
DMANAGER_OBJS = $(patsubst $(SRC_DMANAGER_DIR)%.cpp, $(MAIN_BIN_DIR)%.o, $(wildcard $(SRC_DMANAGER_DIR)*.cpp))
STATISTIK_OBJS= $(patsubst $(SRC_STATISTIK_DIR)%.cpp, $(MAIN_BIN_DIR)%.o, $(wildcard $(SRC_STATISTIK_DIR)*.cpp))
STATMODELS_OBJS= $(patsubst $(SRC_STATMODELS_DIR)%.cpp, $(MAIN_BIN_DIR)%.o, $(wildcard $(SRC_STATMODELS_DIR)*.cpp))
CLUSTERING_OBJS= $(patsubst $(SRC_CLUSTERING_DIR)%.cpp, $(MAIN_BIN_DIR)%.o, $(wildcard $(SRC_CLUSTERING_DIR)*.cpp))
REGRESS_OBJS  = $(patsubst $(SRC_REGRESS_DIR)%.cpp, $(MAIN_BIN_DIR)%.o, $(wildcard $(SRC_REGRESS_DIR)*.cpp))
REDUCT_OBJS   = $(patsubst $(SRC_REDUCT_DIR)%.cpp, $(MAIN_BIN_DIR)%.o, $(wildcard $(SRC_REDUCT_DIR)*.cpp))
AAMODELS_OBJS = $(patsubst $(SRC_AAMODELS_DIR)%.cpp, $(MAIN_BIN_DIR)%.o, $(wildcard $(SRC_AAMODELS_DIR)*.cpp))

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
$(STKPP_LIB): obj
	$(AR) $(ARFLAGS) $@ $(CPP_OBJS)

#-----------------------------------------------------------------------
# rules for cleaning objects
.PHONY: clean_lib clean_obj clean cleanStkernel cleanArrays cleanSdk \
  cleanDmanager cleanAlgebra cleanAnalysis cleanStatistik cleanStatmodel \
  cleanClustering  cleanAAmodels cleanRegress cleanReduct

clean: clean_lib clean_obj

clean_lib:
	$(RM) $(STKPP_LIB)

clean_obj: \
  cleanStkernel \
  cleanArrays \
  cleanSdk \
  cleanDmanager \
  cleanAlgebra \
  cleanAnalysis \
  cleanStatistik \
  cleanStatmodel \
  cleanClustering \
  cleanAAmodels \
  cleanRegress \
  cleanReduct

cleanStkernel:
	$(RM) $(STKERNEL_OBJS)
cleanArrays:
	$(RM) $(ARRAYS_OBJS)
cleanSdk:
#	$(RM) $(SDK_OBJS)
cleanDmanager:
	$(RM) $(DMANAGER_OBJS)
cleanAlgebra:
	$(RM) $(ALGEBRA_OBJS)
cleanAnalysis:
	$(RM) $(ANALYSIS_OBJS)
cleanStatistik:
	$(RM) $(STATISTIK_OBJS)
cleanStatmodel:
	$(RM) $(STATMODELS_OBJS)
cleanClustering:
	$(RM) $(CLUSTERING_OBJS)
cleanAAmodels:
	$(RM) $(AAMODELS_OBJS)
cleanRegress:
	$(RM) $(REGRESS_OBJS)
cleanReduct:
	$(RM) $(REDUCT_OBJS)

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
.PHONY: testsAAModels testsCsv testsArrays testsAlgebra testsDManager testsStatModels \
testsRegress testsSTatistiK testsPages all_tests testsClustering cleantests

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

all_tests: testsSTKernel testsArrays testsAnalysis testsAlgebra testsDManager \
testsStatModels testsRegress testsAAModels testsSTatistiK testsClustering

.PHONY: cleantestsAAModels cleantestArrays cleantestAlgebra cleantestDManager \
cleantestsStatModels cleantestRegress cleantestsSTatistiK cleantestClustering

cleantestsSTKernel:
	cd $(TESTS_STKERNEL_DIR); $(MAKE) clean
cleantestsArrays:
	cd $(TESTS_ARRAYS_DIR); $(MAKE) clean
cleantestsAnalysis:
	cd $(TESTS_ANALYSIS_DIR); $(MAKE) clean
cleantestsAlgebra:
	cd $(TESTS_ALGEBRA_DIR); $(MAKE) clean
cleantestsRegress:
	cd $(TESTS_REGRESS_DIR); $(MAKE) clean
cleantestsDManager:
	cd $(TESTS_DMANAGER_DIR); $(MAKE) clean
cleantestsSTatistiK:
	cd $(TESTS_STATISTIK_DIR); $(MAKE) clean
cleantestsStatModels:
	cd $(TESTS_STATMODELS_DIR); $(MAKE) clean
cleantestsAAModels:
	cd $(TESTS_AAMODELS_DIR); $(MAKE) clean
cleantestsClustering:
	cd $(TESTS_CLUSTERING_DIR); $(MAKE) clean

clean_tests: cleantestsSTKernel cleantestsArrays cleantestsAnalysis cleantestsAlgebra cleantestsDManager \
cleantestsStatModels cleantestsRegress cleantestsAAModels cleantestsSTatistiK cleantestsClustering

#-----------------------------------------------------------------------
# Tutorials and Programs
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
	$(RM) $(MAIN_BIN_DIR)tuto*
cleanmixbin:
	cd $(MIXBIN_DIR); $(MAKE) clean
cleancontiguity:
	cd $(CONTIGUITY_DIR); $(MAKE) clean

#-----------------------------------------------------------------------
# gui project (to remove)
.PHONY: qt guimodel gui
qt:
	cd ./gui/qt/; qmake; $(MAKE)

guimodel:
	cd ./gui/model/src; $(MAKE) all

gui: guimodel qt
