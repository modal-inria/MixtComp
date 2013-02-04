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
# Purpose:  Makefile of the tutos
# Author:   Serge Iovleff, serge.iovleff@stkpp.org
#
#-----------------------------------------------------------------------
# makevars contains general definitions and flags.
#
include ../../makevars

#-----------------------------------------------------------------------
# overide Paths BIN and LIB
#
BIN_DIR = ../../bin/
LIB_DIR = ../../lib/

#-----------------------------------------------------------------------
# source files
#
CPP_SRCS = \
	tutoAccessors.cpp \
	tutoSubArrays.cpp \
	tutoReference.cpp \
	tutoConstructors.cpp

## library name
CXX_LIB = $(LIB_DIR)/$(LIB_STKPP_A)

#-----------------------------------------------------------------------
# binary files
#
CXX_OBJS = $(CPP_SRCS:%.cpp=$(BIN_DIR)%.o)

#-----------------------------------------------------------------------
# program files
#
CXX_PROGS = $(CPP_SRCS:%.cpp=$(BIN_DIR)%)

#-----------------------------------------------------------------------
# Targets
#
.PHONY: all obj
all: obj tutoprograms

obj: $(CXX_OBJ)
tutoprograms: $(CXX_PROGS)

#-----------------------------------------------------------------------
# Objects compilation rules
#
$(BIN_DIR)%.o: makefile

$(BIN_DIR)%.o: %.cpp
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(STK_FLAGS) $< -c -o $@


#-----------------------------------------------------------------------
# program compilation rules
#
$(BIN_DIR)tuto%: makefile
$(BIN_DIR)tuto%: $(BIN_DIR)tuto%.o $(CXX_LIB)
	$(CXX) $< $(CXX_LIB) -o $@ -lm

#-----------------------------------------------------------------------
# clean test programs 
clean:
	$(RM) $(BIN_DIR)tuto*
