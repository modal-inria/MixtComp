CXX = clang++
CXXFLAGS += -O2 -g -std=c++11 -Wall -DMC_VERBOSE

#
# generic build variables
CXXFLAGS += -fopenmp -pthread -MD -MP -DEIGEN_MATRIXBASE_PLUGIN=\"mixt_EigenMatrixBaseAddons.h\" -fPIC 
CXXFLAGS += -I../MixtComp/src -I../MixtComp/src/LinAlg -I../boost -I../eigen -I../gtest -I../gtest/include -I../nlopt/api -I../json-develop/src

# library files
LIB_MC = ../MixtComp/lib/libMixtComp.a
LIB_JMC = lib/libJsonMixtComp.a

# regex files
LIB_RE = lib/libRegex.a
SRC_RE = $(wildcard ../boost/libs/regex/src/*.cpp)
O_RE = $(SRC_RE:%.cpp=%.o)

# optimization library
LIB_NLOPT = ../nlopt/.libs/libnlopt.a

# unit-test files
EXE_UTEST = utest/utest
SRC_GTEST = ../gtest/src/gtest_main.cc ../gtest/src/gtest-all.cc
SRC_UTEST += $(wildcard utest/*.cpp)

O_GTEST = $(SRC_GTEST:%.cc=%.o)
O_UTEST = $(SRC_UTEST:%.cpp=%.o)

SRC_JMC += $(wildcard src/*.cpp)
O_JMC = $(SRC_JMC:%.cpp=%.o)

R_BUILD = T # Variable used to signal that the compilation occurs from inside R
export R_BUILD CXX CXXFLAGS

all: $(O_JMC) $(LIB_MC) $(LIB_RE) $(LIB_NLOPT) 
	$(CXX) $(CXXFLAGS) $^ -o JsonMixtcomp
		 
$(LIB_MC): 
	cd ../MixtComp; make lib
	
clean: # only cleans JMixtComp
	find -L . -type f -name "*.o" -exec rm -f {} \;
	find -L . -type f -name "*.d" -exec rm -f {} \;
	find -L . -type f -name "*.a" -exec rm -f {} \;
	find -L . -type f -name "*.so" -exec rm -f {} \;
	find -L . -type f -name "*.exe" -exec rm -f {} \;
	find -L . -type f -name "*.dll" -exec rm -f {} \;

cleanUTest: # only cleans utest
	find -L utest -type f -name "*.o" -exec rm -f {} \;
	find -L utest -type f -name "*.d" -exec rm -f {} \;
	find -L utest -type f -name "*.a" -exec rm -f {} \;
	find -L utest -type f -name "*.so" -exec rm -f {} \;
	find -L utest -type f -name "*.exe" -exec rm -f {} \;
	find -L utest -type f -name "*.dll" -exec rm -f {} \;

cleanAll: clean # only cleans JMixtComp
	cd ../MixtComp; make cleanAll
	

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
	
-include $(SRC_JMC:%.cpp=%.d)
-include $(SRC_GTEST:%.cc=%.d)

