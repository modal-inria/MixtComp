# R_HOME = $(shell R RHOME)
R_HOME := C:/R/R-3.1.1
R_ARCH := --arch x64

## include headers and libraries for R
RCPPFLAGS := 		$(shell $(R_HOME)/bin/R $(R_ARCH) CMD config --cppflags)
RLDFLAGS := 		$(shell $(R_HOME)/bin/R $(R_ARCH) CMD config --ldflags)
RBLAS := 		$(shell $(R_HOME)/bin/R $(R_ARCH) CMD config BLAS_LIBS)
RLAPACK := 		$(shell $(R_HOME)/bin/R $(R_ARCH) CMD config LAPACK_LIBS)

## include headers and libraries for Rcpp interface classes
RCPPINCL := 		$(shell echo 'Rcpp:::CxxFlags()' | $(R_HOME)/bin/R $(R_ARCH) --vanilla --slave)
RCPPLIBS := 		$(shell echo 'Rcpp:::LdFlags()'  | $(R_HOME)/bin/R $(R_ARCH) --vanilla --slave)

## include headers and libraries for RInside embedding classes
RINSIDEINCL := 		$(shell echo 'RInside:::CxxFlags()' | $(R_HOME)/bin/R $(R_ARCH) --vanilla --slave)
RINSIDELIBS := 		$(shell echo 'RInside:::LdFlags()'  | $(R_HOME)/bin/R $(R_ARCH) --vanilla --slave)

## compiler etc settings used in default make rules
CXX := 			$(shell $(R_HOME)/bin/R $(R_ARCH) CMD config CXX)
CPPFLAGS := 		-Wall $(shell $(R_HOME)/bin/R $(R_ARCH) CMD config CPPFLAGS)
CXXFLAGS := 		$(RCPPFLAGS) $(RCPPINCL) $(RINSIDEINCL) $(shell $(R_HOME)/bin/R $(R_ARCH) CMD config CXXFLAGS)
LDFLAGS	= 		# -s
LDLIBS := 		$(RLDFLAGS) $(RBLAS) $(RLAPACK) $(RINSIDELIBS) $(RCPPLIBS)
CC := 			$(shell $(R_HOME)/bin/R $(R_ARCH) CMD config CXX)

all:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) test.cpp $(LDFLAGS) $(LDLIBS) -o test
#	g++ -m64 -Wall  -IC:/R/R-3.1.0/include -IC:/R/R-3.1.0/include/x64 -Ic:/rlibs/Rcpp/include -Ic:/rlibs/RInside/include -g -O0 -Wall -mtune=core2 src/test/test.cpp -LC:/R/R-3.1.0/bin/x64 -lR -lm -LC:/R/R-3.1.0/bin/x64 -lRblas -LC:/R/R-3.1.0/bin/x64 -lRlapack "c:/rlibs/RInside/lib/x64/libRInside.a"  -o src/test/test
#	$(CXX) $(ALL_CPPFLAGS) $(ALL_CXXFLAGS) `R CMD config --cppflags` `Rscript -e "Rcpp:::CxxFlags()"` `Rscript -e "RInside:::CxxFlags()"` src/test/test.cpp `R CMD config --ldflags` `Rscript -e "RInside:::LdFlags()"` -o src/test/test
# 	Rscript src/test/test.R

clean:
	rm -f test