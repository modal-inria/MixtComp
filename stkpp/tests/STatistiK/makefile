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
ifneq ($(wildcard ./makevars.local),) 
	include makevars.local
endif

#-----------------------------------------------------------------------
# source files
CXX_SRCS =

TEST_SRCS = \
	testUnivariateReal.cpp \
	testStatFunctors.cpp \
	testLawGamma.cpp

#-----------------------------------------------------------------------
# binary files
CXX_OBJS = $(CXX_SRCS:%.cpp=$(TEST_BIN_DIR)%.o)
TEST_OBJS = $(TEST_SRCS:%.cpp=$(TEST_BIN_DIR)%.o)

#-----------------------------------------------------------------------
# program files
TEST_PROGS = $(TEST_SRCS:%.cpp=$(TEST_BIN_DIR)%)

#-----------------------------------------------------------------------
# Targets
.PHONY: all obj progs
all: progs

progs: obj $(TEST_PROGS)
obj: $(CXX_OBJS) $(TEST_OBJS)

#-----------------------------------------------------------------------
# test testGamma object file 
$(TEST_BIN_DIR)testGamma.o: testGamma.cpp makefile
	@ case x$(HASRMATH) in xt ) \
	echo compiling $@; \
	$(CXX) $(CXXFLAGS) $(STK_CXXFLAGS) -I$(TEST_PROJECTS_DIR) -I$(TEST_INCLUDE_DIR) $(R_FLAGS) $< -c -o $@; \
esac

# other object files 
$(TEST_BIN_DIR)%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(STK_CXXFLAGS) -I$(TEST_PROJECTS_DIR) -I$(TEST_INCLUDE_DIR) $(STK_CXXFLAGS) $< -c -o $@


#-----------------------------------------------------------------------
# rule for compiling the testLawGamma object file
$(TEST_BIN_DIR)testLawGamma: $(TEST_BIN_DIR)testLawGamma.o $(TEST_STKPP_LIB)
	@ case x$(HASRMATH) in xt ) \
	echo compiling $@; \
	$(CXX)  $< $(LDFLAGS) $(R_FLAGS) $(LIB_RMATH_A) $(TEST_STKPP_LIB) -o $@  $(LDLIBS); \
esac

# other program compilation rules
$(TEST_BIN_DIR)test%: $(TEST_BIN_DIR)test%.o $(CXX_OBJS)
	$(CXX) $^ $(TEST_STKPP_LIB) $(LDFLAGS) $(LDLIBS) -o $@ 

#-----------------------------------------------------------------------
# clean test programs 
clean:
	$(RM) $(TEST_PROGS) $(CXX_OBJS) $(TEST_OBJS)
