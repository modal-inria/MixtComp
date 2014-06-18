all:
	R CMD INSTALL --build .
	g++ -I/usr/share/R/include `Rscript -e "Rcpp:::CxxFlags()"` `Rscript -e "RInside:::CxxFlags()"` -O0 -pipe -g -Wall src/test/test.cpp -L/usr/lib/R/lib -lR -lblas -llapack `Rscript -e "RInside:::LdFlags()"` -o src/test/test

allWin:
	Rcmd.exe INSTALL --no-multiarch --with-keep.source .
	g++ -I/usr/share/R/include `Rscript -e "Rcpp:::CxxFlags()"` `Rscript -e "RInside:::CxxFlags()"` -O0 -pipe -g -Wall src/test/test.cpp -L/usr/lib/R/lib -lR -lblas -llapack `Rscript -e "RInside:::LdFlags()"` -o src/test/test
	
cleanAll:
	find .. -type f -path  "*.o" -exec rm -f {} \;
	find .. -type f -path  "*.d" -exec rm -f {} \;
	find .. -type f -path  "*.a" -exec rm -f {} \;
	find .. -type f -path  "*.so" -exec rm -f {} \;
	
cleanRMC:
	rm -f src/mixtCompCluster.o
	rm -f src/mixtSimpleSampler.o
	rm -f src/RcppExports.o
	rm -f src/RMixtComp.so
	rm -f src/test/test
	find .. -type f -path "MixtComp/*.o" -exec rm -f {} \;
	find .. -type f -path "MixtComp/*.d" -exec rm -f {} \;
	find .. -type f -path "MixtComp/*.a" -exec rm -f {} \;
	find .. -type f -path "MixtComp/*.so" -exec rm -f {} \;
