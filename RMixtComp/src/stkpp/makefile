#-----------------------------------------------------------------------
#     Copyright (C) 2004-2013  Serge Iovleff
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
# Macros and definitions
RELATIVEMAKEVARS = ./makevars
MAKEVARS = $(realpath $(RELATIVEMAKEVARS))
export MAKEVARS

include $(MAKEVARS)

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
AAMODELS_DIR  = $(PROJECTS_DIR)AAModels/
REGRESS_DIR   = $(PROJECTS_DIR)Regress/
REDUCT_DIR    = $(PROJECTS_DIR)Reduct/
CLUSTERING_DIR= $(PROJECTS_DIR)Clustering/

SRC_STKERNEL_DIR  = $(STKERNEL_DIR)src/
SRC_ARRAYS_DIR    = $(ARRAYS_DIR)src/
SRC_SDK_DIR       = $(SDK_DIR)src/
SRC_DMANAGER_DIR  = $(DMANAGER_DIR)src/
SRC_ALGEBRA_DIR   = $(ALGEBRA_DIR)src/
SRC_ANALYSIS_DIR  = $(ANALYSIS_DIR)src/
SRC_STATISTIK_DIR = $(STATISTIK_DIR)src/
SRC_STATMODELS_DIR= $(STATMODELS_DIR)src/
SRC_REGRESS_DIR   = $(REGRESS_DIR)src/
SRC_REDUCT_DIR    = $(REDUCT_DIR)src/
SRC_AAMODELS_DIR  = $(AAMODELS_DIR)src/
SRC_CLUSTERING_DIR= $(CLUSTERING_DIR)src/

#-----------------------------------------------------------------------
# projects binaries
STKERNEL_OBJS  = $(patsubst $(SRC_STKERNEL_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_STKERNEL_DIR)/*.cpp))
ARRAYS_OBJS    = $(patsubst $(SRC_ARRAYS_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_ARRAYS_DIR)/*.cpp))
SDK_OBJS       = $(patsubst $(SRC_SDK_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_SDK_DIR)/*.cpp))
ALGEBRA_OBJS   = $(patsubst $(SRC_ALGEBRA_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_ALGEBRA_DIR)/*.cpp))
ANALYSIS_OBJS  = $(patsubst $(SRC_ANALYSIS_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_ANALYSIS_DIR)/*.cpp))
DMANAGER_OBJS  = $(patsubst $(SRC_DMANAGER_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_DMANAGER_DIR)/*.cpp))
STATISTIK_OBJS = $(patsubst $(SRC_STATISTIK_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_STATISTIK_DIR)/*.cpp))
STATMODELS_OBJS= $(patsubst $(SRC_STATMODELS_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_STATMODELS_DIR)/*.cpp))
REGRESS_OBJS   = $(patsubst $(SRC_REGRESS_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_REGRESS_DIR)/*.cpp))
REDUCT_OBJS    = $(patsubst $(SRC_REDUCT_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_REDUCT_DIR)/*.cpp))
AAMODELS_OBJS  = $(patsubst $(SRC_AAMODELS_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_AAMODELS_DIR)/*.cpp))
CLUSTERING_OBJS= $(patsubst $(SRC_CLUSTERING_DIR)%.cpp, $(BIN_DIR)%.o, $(wildcard $(SRC_CLUSTERING_DIR)/*.cpp))

#-----------------------------------------------------------------------
# COMMENT or UNCOMMENT the projects you need for your package
# all binaries you need
CPP_OBJS = \
  $(STKERNEL_OBJS) \
  $(ARRAYS_OBJS) \
  $(SDK_OBJS) \
  $(DMANAGER_OBJS) \
  $(ALGEBRA_OBJS) \
  $(ANALYSIS_OBJS) \
  $(STATISTIK_OBJS) \
  $(STATMODELS_OBJS) \
  $(CLUSTERING_OBJS) #\
#  $(REGRESS_OBJS) \
#  $(REDUCT_OBJS) \
#  $(AAMODELS_OBJS)

#-----------------------------------------------------------------------
# rules for compiling each projects
.PHONY: stkernel arrays sdk dmanager algebra analysis statistik statmodel \
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
aamodels:
	$(MAKE) -C $(SRC_AAMODELS_DIR)
regress:
	$(MAKE) -C $(SRC_REGRESS_DIR)
reduct:
	$(MAKE) -C $(SRC_REDUCT_DIR)
statmodel:
	$(MAKE) -C $(SRC_STATMODELS_DIR)
clustering:
	$(MAKE) -C $(SRC_CLUSTERING_DIR)

#-----------------------------------------------------------------------
# all compiles the objects and create the library, no tests are build
.PHONY: all obj lib
all: lib

#-----------------------------------------------------------------------
# Create the library. Build the objects of each projects and then add them
lib: obj $(STK_LIB)

# obj compiles the objects of the projects you need
obj: stkernel sdk dmanager arrays algebra analysis statistik statmodel clustering #\
#     aamodels regress reduct

#-----------------------------------------------------------------------
# create the library adding the objects
$(STK_LIB): $(CPP_OBJS)
	$(AR) $(ARFLAGS) $@ $(CPP_OBJS)

#-----------------------------------------------------------------------
# rules for cleaning objects
.PHONY: clean_lib clean_obj clean clean_stkernel clean_arrays clean_sdk \
  clean_dmanager clean_algebra clean_analysis clean_statistik clean_statmodel \
  clean_clustering clean_aamodels clean_regress clean_reduct

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
