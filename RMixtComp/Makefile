all:
	Rscript -e "Rcpp::compileAttributes()"
	R CMD INSTALL --no-multiarch --with-keep.source .

clean:
	find src -type f -name '*.o' -delete

updatelib:
	python3 updateLib.py
