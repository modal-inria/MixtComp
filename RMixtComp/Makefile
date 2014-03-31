all:
	R CMD INSTALL --build .
	g++ -I/usr/share/R/include `Rscript -e "Rcpp:::CxxFlags()"` `Rscript -e "RInside:::CxxFlags()"` -O0 -pipe -g -Wall src/test/test.cpp -L/usr/lib/R/lib -lR -lblas -llapack `Rscript -e "RInside:::LdFlags()"` -o src/test/test

cleanAll:
	find -L . -type f -name "*.o" -exec rm -f {} \;
	find -L . -type f -name "*.d" -exec rm -f {} \;
	find -L . -type f -name "*.a" -exec rm -f {} \;
	find -L . -type f -name "*.so" -exec rm -f {} \;
	
cleanRMC:
	rm -f src/mixtCompCluster.o
	rm -f src/mixtSimpleSampler.o
	rm -f src/RcppExports.o
	rm -f src/RMixtComp.so
	rm -f src/test/test
	find -L . -type f -wholename "./src/MixtComp/*.o" -exec rm -f {} \;
	find -L . -type f -wholename "./src/MixtComp/*.d" -exec rm -f {} \;
	find -L . -type f -wholename "./src/MixtComp/*.a" -exec rm -f {} \;
	find -L . -type f -wholename "./src/MixtComp/*.so" -exec rm -f {} \;
