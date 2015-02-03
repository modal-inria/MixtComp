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

include makevars

# build path
LIB_MC = ./lib/libMixtComp.a
BIN_DIR = ./bin
EXE_MC = ./bin/mixtComp
EXE_TEST = ./bin/mcTest
LIB_RE = ./regex/libregex.a

GTEST_DIR = ../gtest
EXE_UNITTEST = ./bin/unitTest
LIB_GTEST = ./lib/gtest_main.a

#-----------------------------------------------------------------------
# build rules

# phony prerequisites are executed unconditionally
.PHONY: all exe obj clean test $(EXE_TEST) $(EXE_UNITTEST)

# all: lib $(LIB_RE) $(EXE_MC) $(EXE_TEST)
all: lib $(LIB_RE) $(EXE_TEST) $(EXE_UNITTEST)

clean:
	find -L . -type f -name "*.o" -exec rm -f {} \;
	find -L . -type f -name "*.d" -exec rm -f {} \;
	find -L . -type f -name "*.a" -exec rm -f {} \;
	find -L . -type f -name "*.so" -exec rm -f {} \;

$(EXE_MC): $(LIB_MC) $(LIB_STK) $(LIB_RE)
	$(CXX) $(CXXFLAGS) -I.. -I../stkpp/projects -I../boost -I../boost src/Prog/mixtCompCluster.cpp $(LIB_MC) $(LIB_STK) $(LIB_RE) -o $(EXE_MC)

$(EXE_TEST): 
	$(CXX) $(CXXFLAGS) -I.. -I../boost -I../eigen test/mcTest.cpp -o $@
	$@

$(EXE_UNITTEST): test/sample1_unittest.cpp $(LIB_GTEST)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR)/include $(CXXFLAGS) -pthread -c test/sample1.cpp -o lib/sample1.o
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR)/include $(CXXFLAGS) -pthread -c test/sample1_unittest.cpp -o lib/sample1_unittest.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread lib/gtest-all.o lib/gtest_main.o lib/sample1.o lib/sample1_unittest.o  -o $@
	$@
  
$(LIB_GTEST):
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GTEST_DIR)/include $(CXXFLAGS) -pthread -c $(GTEST_DIR)/src/gtest-all.cc -o lib/gtest-all.o
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GTEST_DIR)/include $(CXXFLAGS) -pthread -c $(GTEST_DIR)/src/gtest_main.cc -o lib/gtest_main.o
	$(AR) $(ARFLAGS) $@ lib/gtest-all.o lib/gtest_main.o

lib: obj $(LIB_MC)

obj:
	$(MAKE) -C ./src/Composer
	$(MAKE) -C ./src/Data
	$(MAKE) -C ./src/IO
	$(MAKE) -C ./src/Likelihood
	$(MAKE) -C ./src/Mixture
	$(MAKE) -C ./src/Param
	$(MAKE) -C ./src/Sampler
	$(MAKE) -C ./src/Statistic
	$(MAKE) -C ./src/Strategy
	$(MAKE) -C ./src/Various

$(LIB_MC): $(wildcard $(BIN_DIR)/*.o)
	$(AR) $(ARFLAGS) $@ $(BIN_DIR)/*.o
	
$(LIB_STK):
	cd ../stkpp; $(MAKE) all

$(LIB_RE):
	cd regex; $(CXX) $(CXXFLAGS) -MD -MP -fPIC -I../../boost -c ../../boost/libs/regex/src/*.cpp; ar cr libregex.a *.o
