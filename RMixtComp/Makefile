all:
	Rscript -e "Rcpp::compileAttributes()"
	R CMD INSTALL --no-multiarch --with-keep.source .

clean:
	git clean -dfx src

updateLib:
	python3 updateLib.py
