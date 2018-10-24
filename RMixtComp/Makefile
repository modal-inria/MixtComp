all:
	Rscript -e "Rcpp::compileAttributes()"
	R CMD INSTALL --no-multiarch --with-keep.source .

updateLib:
	python3 updateLib.py
