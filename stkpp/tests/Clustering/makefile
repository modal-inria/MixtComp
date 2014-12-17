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
CXX_SRCS = \
	SimulUtil.cpp \
	StrategiesUtil.cpp

TEST_SRCS = \
	testDataHandler.cpp \
	testAllGammaMixtureModels.cpp \
	testAllGaussianMixtureModels.cpp \
	testCategoricalMixtureModels.cpp \
	testCategoricalWithBirds.cpp \
	testHeterogeneousWithHeartDisease.cpp \
	testComposer.cpp
#	testGammaMixtureModels.cpp \

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
# Objects compilation rules
$(TEST_BIN_DIR)%.o: %.cpp 
	$(CXX) $(CXXFLAGS) $(STK_CXXFLAGS) -I$(TEST_PROJECTS_DIR) -I$(TEST_INCLUDE_DIR) $(STK_PROJECT_FLAGS) $< -c -o $@

#-----------------------------------------------------------------------
# program compilation rules
$(TEST_BIN_DIR)test%:  $(CXX_OBJS) $(TEST_BIN_DIR)test%.o
	$(CXX) $^ $(TEST_STKPP_LIB) $(LDFLAGS) $(LDLIBS) -o $@ 

#-----------------------------------------------------------------------
# clean test programs 
.PHONY: clean
clean:
	$(RM) $(TEST_PROGS) $(CXX_OBJS) $(TEST_OBJS)
