CXX = g++
CXXFLAGS += -O2 -g -std=c++11 -Wall

#
# generic build variables
CXXFLAGS += -fopenmp -pthread -MD -MP -DEIGEN_MATRIXBASE_PLUGIN=\"mixt_EigenMatrixBaseAddons.h\" -fPIC 
CXXFLAGS += -I../MixtComp/src -I../MixtComp/src/LinAlg -I../boost -I../eigen -I../gtest -I../gtest/include -I../nlopt/api -I../json-develop/src

# JsonMixtComp exe
EXE_JMC = bin/JsonMixtComp
LIB_JMC = lib/libJsonMixtComp.a
SRC_JMC += $(wildcard src/*.cpp)
O_JMC = $(SRC_JMC:%.cpp=%.o)
D_JMC = $(SRC_JMC:%.cpp=%.d)
ALL_JMC = $(O_JMC) $(D_JMC) $(EXE_JMC) $(LIB_JMC)

# MixtComp library file
LIB_MC = ../MixtComp/lib/libMixtComp.a

# regex files
LIB_RE = ../MixtComp/lib/libRegex.a
SRC_RE = $(wildcard ../boost/libs/regex/src/*.cpp)
O_RE = $(SRC_RE:%.cpp=%.o)
D_RE = $(SRC_RE:%.cpp=%.d)
ALL_RE = $(LIB_RE) $(O_RE) $(D_RE)

# optimization library
ifeq ($(OS),Windows_NT)
	LIB_NLOPT = lib/libnlopt-0.dll
else
	LIB_NLOPT = ../nlopt/.libs/libnlopt.a
endif

# unit-test files
EXE_UTEST = utest/utest
SRC_GTEST = ../gtest/src/gtest_main.cc ../gtest/src/gtest-all.cc
SRC_UTEST += $(wildcard utest/*.cpp)
O_GTEST = $(SRC_GTEST:%.cc=%.o)
O_UTEST = $(SRC_UTEST:%.cpp=%.o)
D_GTEST = $(SRC_GTEST:%.cc=%.d)
D_UTEST = $(SRC_UTEST:%.cc=%.d)
ALL_UTEST = $(SRC_UTEST) $(O_UTEST) $(D_UTEST) $(EXE_UTEST) 
ALL_GTEST = $(SRC_GTEST) $(O_GTEST) $(D_GTEST)

R_BUILD = T # Variable used to signal that the compilation occurs from inside R
export R_BUILD CC CFLAGS CXX CXXFLAGS

all: $(O_JMC) $(LIB_MC) $(LIB_RE) $(LIB_NLOPT) 
	$(CXX) $(CXXFLAGS) $^ -o $(EXE_JMC)
		 
$(LIB_MC): 
	cd ../MixtComp; make lib
	
clean: # only cleans JMixtComp
	rm -f $(ALL_JMC)

cleanGTest:
	rm -f $(ALL_GTEST)

cleanUTest: 
	rm -f $(ALL_UTEST)

cleanNlopt:
	cd ../nlopt;make clean

cleanRe:
	rm -f $(ALL_RE)

cleanAll: clean cleanUTest cleanNlopt cleanRe cleanGTest
	cd ../MixtComp; make clean
	

lib: $(LIB_JMC)

$(LIB_JMC): $(O_JMC)
	$(AR) $(ARFLAGS) $@ $^

$(LIB_RE): $(O_RE)
	$(AR) $(ARFLAGS) $@ $^
	
$(LIB_NLOPT):
	cd ../nlopt;./configure CFLAGS=-fPIC;make

	
utest: $(EXE_UTEST) 

$(EXE_UTEST): $(O_GTEST) $(O_UTEST) $(LIB_JMC) $(LIB_MC) $(LIB_NLOPT) $(LIB_RE) 
	$(CXX) $(CXXFLAGS) $^ -lpthread -o $@ # linker only, as cpp file is compiled by automatic rule
	
-include $(D_JMC)
-include $(D_GTEST)

