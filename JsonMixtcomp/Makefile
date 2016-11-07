CXX = clang++

# generic build variables
CXXFLAGS += -fopenmp -pthread -MD -MP -DEIGEN_MATRIXBASE_PLUGIN=\"LinAlg/mixt_EigenMatrixBaseAddons.h\" -fPIC
CXXFLAGS += -I../MixtComp/src -I../MixtComp/src/LinAlg -I../boost -I../eigen -I../gtest -I../gtest/include -I../nlopt/api 
CXXFLAGS += -std=c++11 

# library files
LIB_MC = ../MixtComp/lib/libMixtComp.a
LIB_JMC = lib/libJsonMixtComp.a

# regex files
LIB_RE = lib/libRegex.a
SRC_RE = $(wildcard ../boost/libs/regex/src/*.cpp)
O_RE = $(SRC_RE:%.cpp=%.o)

# optimization library
LIB_NLOPT = ../nlopt/.libs/libnlopt.a


SRC_JMC += $(wildcard src/*.cpp)
O_JMC = $(SRC_JMC:%.cpp=%.o)

all: $(O_JMC) $(LIB_MC) $(LIB_RE) $(LIB_NLOPT)
	$(CXX) $(CXXFLAGS) $^ -o JsonMixtcomp
		 
$(LIB_MC): 
	cd ../MixtComp; make lib
	
clean: # only cleans MixtComp
	find -L . -type f -name "*.o" -exec rm -f {} \;
	find -L . -type f -name "*.d" -exec rm -f {} \;
	find -L . -type f -name "*.a" -exec rm -f {} \;
	find -L . -type f -name "*.so" -exec rm -f {} \;
	find -L . -type f -name "*.exe" -exec rm -f {} \;
	find -L . -type f -name "*.dll" -exec rm -f {} \;

lib: $(LIB_JMC)

$(LIB_JMC): $(O_JMC)
	$(AR) $(ARFLAGS) $@ $^

$(LIB_RE): $(O_RE)
	$(AR) $(ARFLAGS) $@ $^
	
$(LIB_NLOPT):
	cd ../nlopt;./configure CFLAGS=-fPIC;make
-include $(SRC_JMC:%.cpp=%.d)
