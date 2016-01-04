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
# CXXFLAGS += -pthread -MD -MP -fPIC
CXXFLAGS += -Isrc/LinAlg -I../boost -I../eigen -I../gtest -I../gtest/include
CXXFLAGS += -std=c++11 -DMC_VERBOSE # included here, because it is standard and we do not have access on Makevars on BigStat for example

# library files
LIB_MC = lib/libMixtComp.a
SRC_MC += $(wildcard src/Composer/*.cpp)
SRC_MC += $(wildcard src/Data/*.cpp)
SRC_MC += $(wildcard src/IO/*.cpp)
SRC_MC += $(wildcard src/Likelihood/*.cpp)
SRC_MC += $(wildcard src/LinAlg/*.cpp)
SRC_MC += $(wildcard src/Mixture/Ordinal/*.cpp)
SRC_MC += $(wildcard src/Mixture/Rank/*.cpp)
SRC_MC += $(wildcard src/Mixture/SimpleMixtureBridge/*.cpp)
SRC_MC += $(wildcard src/Param/*.cpp)
SRC_MC += $(wildcard src/Sampler/*.cpp)
SRC_MC += $(wildcard src/Statistic/*.cpp)
SRC_MC += $(wildcard src/Strategy/*.cpp)
SRC_MC += $(wildcard src/Various/*.cpp)
O_MC = $(SRC_MC:%.cpp=%.o)

# regex files
LIB_RE = lib/libRegex.a
SRC_RE = $(wildcard ../boost/libs/regex/src/*.cpp)
O_RE = $(SRC_RE:%.cpp=%.o)

# sample test
EXE_TEST = test/test
SRC_TEST = test/mcTest.cpp
O_TEST = $(SRC_TEST:%.cpp=%.o)

# unit-test files
EXE_UTEST = utest/utest
SRC_GTEST = ../gtest/src/gtest_main.cc ../gtest/src/gtest-all.cc
# SRC_UTEST += $(wildcard utest/Ordinal/*.cpp) $(wildcard utest/Rank/*.cpp) $(wildcard utest/Various/*.cpp)
SRC_UTEST += $(wildcard utest/UTestNew.cpp)
O_GTEST = $(SRC_GTEST:%.cc=%.o)
O_UTEST = $(SRC_UTEST:%.cpp=%.o)

# $(warning $(CXX)) # example of console output for debug
# $(warning $(CXXFLAGS)) # example of console output for debug

# make recipes
clean: # only cleans MixtComp
	find -L . -type f -name "*.o" -exec rm -f {} \;
	find -L . -type f -name "*.d" -exec rm -f {} \;
	find -L . -type f -name "*.a" -exec rm -f {} \;
	find -L . -type f -name "*.so" -exec rm -f {} \;
	find -L . -type f -name "*.exe" -exec rm -f {} \;
	find -L . -type f -name "*.dll" -exec rm -f {} \;

allClean: # cleans MixtComp and all bundled libraries
	find -L .. -type f -name "*.o" -exec rm -f {} \;
	find -L .. -type f -name "*.d" -exec rm -f {} \;
	find -L .. -type f -name "*.a" -exec rm -f {} \;
	find -L .. -type f -name "*.so" -exec rm -f {} \;
	find -L .. -type f -name "*.exe" -exec rm -f {} \;
	find -L .. -type f -name "*.dll" -exec rm -f {} \;

cleanTest:
	find -L test -type f -name "*.o" -exec rm -f {} \;
	find -L test -type f -name "*.d" -exec rm -f {} \;
	find -L test -type f -name "*.a" -exec rm -f {} \;
	find -L test -type f -name "*.so" -exec rm -f {} \;
	find -L test -type f -name "*.exe" -exec rm -f {} \;
	find -L test -type f -name "*.dll" -exec rm -f {} \;

cleanUTest: # only cleans MixtComp
	find -L utest -type f -name "*.o" -exec rm -f {} \;
	find -L utest -type f -name "*.d" -exec rm -f {} \;
	find -L utest -type f -name "*.a" -exec rm -f {} \;
	find -L utest -type f -name "*.so" -exec rm -f {} \;
	find -L utest -type f -name "*.exe" -exec rm -f {} \;
	find -L utest -type f -name "*.dll" -exec rm -f {} \;

lib: $(LIB_MC)

$(LIB_MC): $(O_MC)
	$(AR) $(ARFLAGS) $@ $^

libRE: $(LIB_RE)

$(LIB_RE): $(O_RE)
	$(AR) $(ARFLAGS) $@ $^
	
utest: $(EXE_UTEST)

#$(EXE_UTEST): $(O_GTEST) $(O_UTEST)
#	$(CXX) $^ $(LIB_MC) -o $@ -lpthread 

$(EXE_UTEST): $(LIB_MC) $(LIB_RE) $(O_GTEST) $(O_UTEST)
	$(CXX) $^ $(LIB_MC) $(LIB_RE) -lpthread -o $@ # linker only, as cpp file is compiled by automatic rule

test: $(EXE_TEST)

$(EXE_TEST): $(O_TEST)
	$(CXX) $^ -lpthread -o $@ # linker only, as cpp file is compiled by automatic rule

-include $(SRC_MC:%.cpp=%.d)
-include $(SRC_RE:%.cpp=%.d)
-include $(SRC_TEST:%.cpp=%.d)
-include $(SRC_GTEST:%.cc=%.d)
-include $(SRC_UTEST:%.cpp=%.d)