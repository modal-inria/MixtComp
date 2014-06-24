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
# Purpose:  Makefile of the tests
# Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
#
#-----------------------------------------------------------------------
# makevars contains general definitions and flags.
ifeq ($(MAKEVARS),)
        include ../../$(RELATIVEMAKEVARS)
else
        include $(MAKEVARS)
endif
ifneq ($(wildcard ./makevars.local),) 
	include makevars.local
endif

#-----------------------------------------------------------------------
# Paths to bin, lib, include and projects
BIN_DIR = ../../bin/
LIB_DIR = ../../lib/
INC_DIR = ../../include/
PROJECTS_DIR = ../../projects/

ARRAYS_INC_DIR = ../../projects/Arrays/include/

#-----------------------------------------------------------------------
# source files
#
CPP_SRCS = \
	testArray1D.cpp \
	testArray2D.cpp \
	testArray2DMergeSet.cpp \
	testArray2DDiagonal.cpp \
	testArray2DResize.cpp \
	testCAllocator.cpp \
	testCArrays.cpp \
	testCArrayAssign.cpp \
	testConstArrays.cpp \
	testUnaryOperators.cpp \
	testBinaryOperators.cpp \
	testSliceOperators.cpp \
	testProducts.cpp \
	testVisitors.cpp \
	testVisitorsUnrolled.cpp \
	testBiProducts.cpp \
	testBiSum.cpp

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
all: obj progs

obj: $(CXX_OBJS) $(CPP_SRCS)
progs: $(CXX_PROGS)

#-----------------------------------------------------------------------
# Objects compilation rules
#
$(BIN_DIR)%.o: makefile
$(BIN_DIR)testArray2D.o: $(ARRAYS_INC_DIR)STK_Array2DVector.h \
                         $(ARRAYS_INC_DIR)STK_Array2DPoint.h \
                         $(ARRAYS_INC_DIR)STK_Array2D.h \
                         $(ARRAYS_INC_DIR)STK_IArray2DBase.h \
                         $(ARRAYS_INC_DIR)STK_Array2DDiagonal.h \
                         $(ARRAYS_INC_DIR)STK_IArray2D.h

$(BIN_DIR)%.o: %.cpp makefile $(CXX_LIB)
	$(CXX) $(CFLAGS) $(CXXFLAGS) -I$(PROJECTS_DIR) -I$(INC_DIR) $(STK_FLAGS) $< -c -o $@


#-----------------------------------------------------------------------
# program compilation rules
#
$(BIN_DIR)test%: makefile
$(BIN_DIR)test%: $(BIN_DIR)test%.o
	$(CXX) $(LDFLAGS) $< $(CXX_LIB) $(LDLIBS) -o $@ 

#-----------------------------------------------------------------------
# clean test programs 
clean:
	$(RM) $(BIN_DIR)test*
