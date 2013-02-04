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
# Purpose:  Makefile of the tests
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

DMANAGER_INC_DIR  = ../../projects/DManager/include/

#-----------------------------------------------------------------------
# source files
#
CPP_SRCS = \
	testList1D.cpp \
	testHeapSort.cpp \
	testCsv.cpp \
	testPages.cpp 

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
.PHONY: all obj testCsv
all: obj testprograms

obj: $(CXX_OBJ)
testprograms: $(CXX_PROGS)

testCsv: $(BIN_DIR)testCsv
testPages: $(BIN_DIR)testPages

#-----------------------------------------------------------------------
# Objects compilation rules
#
$(BIN_DIR)%.o: makefile
$(BIN_DIR)testList1D.o: $(DMANAGER_INC_DIR)STK_List1D.h

$(BIN_DIR)%.o: %.cpp
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(STK_FLAGS) $< -c -o $@


#-----------------------------------------------------------------------
# program compilation rules
#
$(BIN_DIR)test%: makefile
$(BIN_DIR)test%: $(BIN_DIR)test%.o $(CXX_LIB)
	$(CXX) $< $(CXX_LIB) -o $@ -lm

#-----------------------------------------------------------------------
# clean test programs 
clean:
	$(RM) $(BIN_DIR)test*

