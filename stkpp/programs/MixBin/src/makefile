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
# Purpose:  Makefile of the MixBin application
# Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
#
#-----------------------------------------------------------------------
# aam macros and definitions
include ../../../makevars

#-----------------------------------------------------------------------
# Paths
#
BIN_DIR = ../bin/

STKPP_DIR = ../../../
STKPP_LIB_DIR = $(STKPP_DIR)lib/
STKPP_INC_DIR = $(STKPP_DIR)include/

STKPP_LIB = $(STKPP_LIB_DIR)$(LIB_STKPP_A)

#-----------------------------------------------------------------------
# header files
#
CXX_INCS	= $(wildcard $(INC_DIR)*.h)

#-----------------------------------------------------------------------
# source files
#
CPP_SRCS = $(wildcard *.cpp)

#-----------------------------------------------------------------------
# binary files
#
CPP_OBJS = $(CPP_SRCS:%.cpp=$(BIN_DIR)%.o)

#-----------------------------------------------------------------------
# Compilation rules
#
$(BIN_DIR)%.o: $(INC_DIR)%.h makefile
$(BIN_DIR)%.o: %.cpp
	$(CXX) $(CFLAGS) $(CXXFLAGS) -I$(STKPP_INC_DIR) $(AAM_FLAGS) $< -c -o $@

#-----------------------------------------------------------------------
# rule for compiling the aam program
mixbin: $(CPP_OBJS) $(CXX_INCS)
	$(CXX)  $(CFLAGS) $(CXXFLAGS) $(CPP_OBJS) $(STKPP_LIB) \
	-o $(BIN_DIR)$@ -lm

#-----------------------------------------------------------------------
# creating objects files
#
obj: $(CPP_OBJS)

#-----------------------------------------------------------------------
# targets
all: mixbin

#-----------------------------------------------------------------------
# targets
clean:
	cd $(BIN_DIR); rm -rf *.o mixbin