all:
	R CMD INSTALL --build .

clean:
	rm src/*.so src/*.o src/test/test

test:
	g++ -I/usr/share/R/include -I/home/modal/R/x86_64-pc-linux-gnu-library/3.0/Rcpp/include -I/home/modal/R/x86_64-pc-linux-gnu-library/3.0/RInside/include -O0 -pipe -g -Wall src/test/test.cpp -L/usr/lib/R/lib -lR -lblas -llapack -L/home/modal/R/x86_64-pc-linux-gnu-library/3.0/RInside/lib -lRInside -Wl,-rpath,/home/modal/R/x86_64-pc-linux-gnu-library/3.0/RInside/lib -o src/test/test
	