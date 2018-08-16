- http://gallery.rcpp.org/articles/rcpp-wrap-and-recurse/
- https://github.com/wch/r-source/blob/48536f1756a88830076023db9566fbb2c1dbb29b/src/include/Rinternals.h#L1178-L1214, fonction de détection
- Types to be used for detection: https://github.com/wch/r-source/blob/48536f1756a88830076023db9566fbb2c1dbb29b/src/include/Rinternals.h#L148
- https://stackoverflow.com/questions/37665361/how-to-determine-the-class-of-object-stored-in-sexp-in-rcpp
- Set attributes in Rcpp: http://gallery.rcpp.org/articles/setting-object-attributes/,
- Type, mode, class: recap table: https://stackoverflow.com/questions/8855589/a-comprehensive-survey-of-the-types-of-things-in-r-mode-and-class-and-type/40171527#40171527

Note that R does not make any difference between scalar and vector types. The SGraph does, because an AlgType can be a Real or a NamedVector<Real>. The convention used is that if the R NumericVector is of length(1) and has no names, it is a scalar. Otherwise, it is a vector.
