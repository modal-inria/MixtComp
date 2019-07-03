# MixtComp version 4 - july 2019
# Copyright (C) Inria - Université de Lille - CNRS 

# Note that this Makefile will not be available on CRAN. Make commands should not be called from configure or cleanup scripts.

all:
	Rscript -e "Rcpp::compileAttributes()"
	R CMD INSTALL --no-multiarch --with-keep.source .

clean:
	find src -type f -name '*.o' -delete

updateLib:
	python3 updateLib.py
