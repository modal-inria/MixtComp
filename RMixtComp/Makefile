all:
#	R CMD INSTALL --build .
	Rscript -e "Rcpp::compileAttributes()"
	R CMD INSTALL --no-multiarch --with-keep.source .
#	cd src/test;make all

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
