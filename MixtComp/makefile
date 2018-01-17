#------------------------------------------------------------------------
#   Copyright (C) Inria 2014
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#------------------------------------------------------------------------
#   Project:    MixtComp
#   Created on: Feb 17, 2014
#   Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
#------------------------------------------------------------------------

# test wether the build was triggered by R or not, if not then get the compilation flags from a file
ifeq ($(R_BUILD),)
include Makevars
endif

# generic build variables
CXXFLAGS += -pthread -MD -MP -DEIGEN_MATRIXBASE_PLUGIN=\"mixt_EigenMatrixBaseAddons.h\" -fPIC
CXXFLAGS += -Isrc -Isrc/LinAlg -I../boost -I../eigen -I../gtest -I../gtest/include -I../nlopt/api
CXXFLAGS += -std=c++11 # included here, because it is standard and we do not have access on Makevars on BigStat for example

# library files
LIB_MC = lib/libMixtComp.a
SRC_MC += $(wildcard src/Composer/*.cpp)
SRC_MC += $(wildcard src/Data/*.cpp)
SRC_MC += $(wildcard src/IO/*.cpp)
SRC_MC += $(wildcard src/Likelihood/*.cpp)
SRC_MC += $(wildcard src/LinAlg/*.cpp)
SRC_MC += $(wildcard src/Mixture/Functional/*.cpp)
SRC_MC += $(wildcard src/Mixture/Ordinal/*.cpp)
SRC_MC += $(wildcard src/Mixture/Rank/*.cpp)
SRC_MC += $(wildcard src/Mixture/Simple/*.cpp)
SRC_MC += $(wildcard src/Mixture/SimpleMixtureBridge/*.cpp)
SRC_MC += $(wildcard src/Param/*.cpp)
SRC_MC += $(wildcard src/Sampler/*.cpp)
SRC_MC += $(wildcard src/Statistic/*.cpp)
SRC_MC += $(wildcard src/Strategy/*.cpp)
SRC_MC += $(wildcard src/Various/*.cpp)
O_MC = $(SRC_MC:%.cpp=%.o)
D_MC = $(SRC_MC:%.cpp=%.d)
ALL_MC = $(O_MC) $(D_MC) $(LIB_MC)

# regex files
LIB_RE = lib/libRegex.a
SRC_RE = $(wildcard ../boost/libs/regex/src/*.cpp)
O_RE = $(SRC_RE:%.cpp=%.o)
D_RE = $(SRC_RE:%.cpp=%.d)
ALL_RE = $(O_RE) $(D_RE) $(LIB_RE)

# optimization library
LIB_NLOPT = ../nlopt/.libs/libnlopt.a

# sample test
EXE_TEST = test/test
SRC_TEST = test/03-optimization.cpp
O_TEST = $(SRC_TEST:%.cpp=%.o)
D_TEST = $(SRC_TEST:%.cpp=%.d)
ALL_TEST = $(O_TEST) $(D_TEST) $(EXE_TEST)

# unit-test files
EXE_UTEST = utest/utest
SRC_GTEST = ../gtest/src/gtest_main.cc ../gtest/src/gtest-all.cc
SRC_UTEST += $(wildcard utest/Functional/*.cpp)
SRC_UTEST += $(wildcard utest/LinAlgStat/*.cpp)
SRC_UTEST += $(wildcard utest/Ordinal/*.cpp)
SRC_UTEST += $(wildcard utest/Rank/*.cpp)
SRC_UTEST += $(wildcard utest/Statistic/*.cpp)
SRC_UTEST += $(wildcard utest/Simple/*.cpp)
SRC_UTEST += $(wildcard utest/SimpleMixture/*.cpp)
SRC_UTEST += $(wildcard utest/Various/*.cpp)
SRC_UTEST += $(wildcard utest/UTestNew.cpp)
O_GTEST = $(SRC_GTEST:%.cc=%.o)
O_UTEST = $(SRC_UTEST:%.cpp=%.o)
D_GTEST = $(SRC_GTEST:%.cc=%.d)
D_UTEST = $(SRC_UTEST:%.cpp=%.d)
ALL_GTEST = $(O_GTEST) $(D_GTEST)
ALL_UTEST = $(O_UTEST) $(D_UTEST) $(EXE_UTEST)

# $(warning $(CXX)) # example of console output for debug
# $(warning $(CXXFLAGS)) # example of console output for debug

# make recipes
clean: # only cleans MixtComp
	rm -f $(ALL_MC)

cleanAll: clean cleanGTest cleanNlopt cleanRe

cleanGTest:
	rm -f $(ALL_GTEST)

cleanNlopt:
	cd ../nlopt;make clean

cleanRe:
	rm -f $(ALL_RE)

cleanTest:
	rm -f $(ALL_TEST)

cleanUTest: 
	rm -f $(ALL_UTEST)

lib: $(LIB_MC)

$(LIB_MC): $(O_MC)
	$(AR) $(ARFLAGS) $@ $^

$(LIB_RE): $(O_RE)
	$(AR) $(ARFLAGS) $@ $^
	
$(LIB_NLOPT):
	cd ../nlopt;./configure CFLAGS=-fPIC;make
	
utest: $(EXE_UTEST)

$(EXE_UTEST): $(LIB_MC) $(LIB_NLOPT) $(LIB_RE) $(O_GTEST) $(O_UTEST)
	$(CXX) $^ -lpthread -o $@ # linker only, as cpp file is compiled by automatic rule

#test: $(EXE_TEST)

#$(EXE_TEST): $(LIB_MC) $(O_TEST)
#	$(CXX) $^ -lpthread $(LIB_MC) $(LIB_OPT) -o $@ # linker only, as cpp file is compiled by automatic rule

-include $(D_MC)
-include $(D_RE)
-include $(D_TEST)
-include $(D_GTEST)
-include $(D_UTEST)