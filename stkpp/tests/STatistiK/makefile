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
# overide Paths BIN and LIB
#
BIN_DIR = ../../bin/
LIB_DIR = ../../lib/

#-----------------------------------------------------------------------
# source files
#
CPP_SRCS = \
	testUnivariateReal.cpp \
	testStatFunctors.cpp \
	testLawGamma.cpp

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
# test testGamma object file 
$(BIN_DIR)testGamma.o: testGamma.cpp makefile
	@ case x$(HASRMATH) in xt ) \
	echo compiling $@; \
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(STK_FLAGS) $(R_FLAGS) $< -c -o $@; \
esac

# other object files 
$(BIN_DIR)%.o: %.cpp
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(STK_FLAGS) $< -c -o $@


#-----------------------------------------------------------------------
# rule for compiling the testLawGamma object file
$(BIN_DIR)testLawGamma: $(BIN_DIR)testLawGamma.o $(CXX_LIB)
	@ case x$(HASRMATH) in xt ) \
	echo compiling $@; \
	$(CXX)  $< $(LDFLAGS) $(R_FLAGS) $(LIB_RMATH_A) $(CXX_LIB) -o $@  $(LDLIBS); \
esac

# other program compilation rules
$(BIN_DIR)test%: makefile
$(BIN_DIR)test%: $(BIN_DIR)test%.o $(CXX_LIB)
	$(CXX) $< $(LDFLAGS) $(CXX_LIB) -o $@  $(LDLIBS) 

#-----------------------------------------------------------------------
# clean test programs 
clean:
	$(RM) $(BIN_DIR)test*
