## include headers and libraries for R
RCPPFLAGS := $(shell R CMD config --cppflags)
RLDFLAGS := $(shell R CMD config --ldflags)
RBLAS := $(shell R CMD config BLAS_LIBS)
RLAPACK := $(shell R CMD config LAPACK_LIBS)

## include headers and libraries for Rcpp interface classes
RCPPINCL := $(shell echo 'Rcpp:::CxxFlags()' | R --vanilla --slave)
RCPPLIBS := $(shell echo 'Rcpp:::LdFlags()'  | R --vanilla --slave)

## include headers and libraries for RInside embedding classes
RINSIDEINCL := $(shell echo 'RInside:::CxxFlags()' | R --vanilla --slave)
RINSIDELIBS := 	$(shell echo 'RInside:::LdFlags()' | R --vanilla --slave)

## compiler etc settings used in default make rules
CXX := $(shell R $(R_ARCH) CMD config CXX)
CPPFLAGS :=	-Wall $(shell R $(R_ARCH) CMD config CPPFLAGS)
CXXFLAGS := $(RCPPFLAGS) $(RCPPINCL) $(RINSIDEINCL) $(shell R CMD config CXXFLAGS)
LDFLAGS	= # -s
LDLIBS := $(RLDFLAGS) $(RBLAS) $(RLAPACK) $(RINSIDELIBS) $(RCPPLIBS)
CC := $(shell R CMD config CXX)

# files for test case
EXE_TEST = test
SRC_TEST = $(wildcard *.cpp)
O_TEST = $(SRC_TEST:%.cpp=%.o)

clean:
	rm -f test

all: $(EXE_TEST)

$(EXE_TEST): $(O_TEST)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ $(LDFLAGS) $(LDLIBS) -o $@

-include $(SRC_TEST:%.cpp=%.d)