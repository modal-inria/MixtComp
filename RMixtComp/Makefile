all:
#	R CMD INSTALL --build .
	R CMD INSTALL --no-multiarch --with-keep.source .
#	g++ -I/usr/share/R/include `Rscript -e "Rcpp:::CxxFlags()"` `Rscript -e "RInside:::CxxFlags()"` -O0 -pipe -g -Wall `Rscript -e "RInside:::LdFlags()"` src/test/test.cpp -L/usr/lib/R/lib -lR -lblas -llapack -o src/test/test

boost:
	cd ../boost_1_56_0/libs/regex/build; rm -Rf gcc; make -f gcc.mak
	
cleanAll:
	find .. -type f -path  "*.o" -exec rm -f {} \;
	find .. -type f -path  "*.d" -exec rm -f {} \;
	find .. -type f -path  "*.a" -exec rm -f {} \;
	find .. -type f -path  "*.so" -exec rm -f {} \;
	find .. -type f -path  "*.exe" -exec rm -f {} \;
	find .. -type f -path  "*.dll" -exec rm -f {} \;
	
cleanRMC:
	rm -f src/*.o
	rm -f src/*.d
	rm -f src/*.a
	rm -f src/*.so
	rm -f src/*.exe
	rm -f src/*.dll
	find .. -type f -path "../MixtComp/*.o" -exec rm -f {} \;
	find .. -type f -path "../MixtComp/*.d" -exec rm -f {} \;
	find .. -type f -path "../MixtComp/*.a" -exec rm -f {} \;
	find .. -type f -path "../MixtComp/*.so" -exec rm -f {} \;

cleanRMCbase:
	rm -f src/*.o
	rm -f src/*.d
	rm -f src/*.a
	rm -f src/*.so
	rm -f src/*.exe
	rm -f src/*.dll

cleanTest:
	rm -f src/test/test.exe