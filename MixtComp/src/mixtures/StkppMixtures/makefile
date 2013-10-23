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
# Purpose:  Generic makefile.
# Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
#
#-----------------------------------------------------------------------
# Macro definitions
#
include $(MAKEVARS)

#-----------------------------------------------------------------------
# Paths
#
INC_DIR = ../include/
BIN_DIR = ../../../bin/

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
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(STK_FLAGS) $< -c -o $@

#-----------------------------------------------------------------------
# creating objects files
#
obj: $(CPP_OBJS)

#-----------------------------------------------------------------------
# Targets
#
all: obj
