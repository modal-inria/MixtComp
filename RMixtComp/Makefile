all:
#	R CMD INSTALL --build .
	Rscript -e "Rcpp::compileAttributes()"
	R CMD INSTALL --no-multiarch --with-keep.source .
#	g++ -I/usr/share/R/include `Rscript -e "Rcpp:::CxxFlags()"` `Rscript -e "RInside:::CxxFlags()"` -O0 -pipe -g -Wall `Rscript -e "RInside:::LdFlags()"` src/test/test.cpp -L/usr/lib/R/lib -lR -lblas -llapack -o src/test/test

cleanAll: cleanMixtComp cleanRMCbase cleanTest

cleanMixtComp:
	cd ../MixtComp; git clean -dfx debug release

cleanRMCbase:
	rm -f src/*.o
	rm -f src/*.d
	rm -f src/*.a
	rm -f src/*.so
	rm -f src/*.exe
	rm -f src/*.dll
	rm -f src/RcppExports.cpp
	rm -f R/RcppExports.R

cleanTest:
	rm -f src/test/test
	rm -f src/test/test.exe
