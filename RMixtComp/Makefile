all:
	R CMD INSTALL --build .
	g++ -I/usr/share/R/include `Rscript -e "Rcpp:::CxxFlags()"` `Rscript -e "RInside:::CxxFlags()"` -O0 -pipe -g -Wall src/test/test.cpp -L/usr/lib/R/lib -lR -lblas -llapack `Rscript -e "RInside:::LdFlags()"` -o src/test/test

clean:
	rm src/*.so src/*.o src/test/test
