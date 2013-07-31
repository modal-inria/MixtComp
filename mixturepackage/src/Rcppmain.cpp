/** @file Rcoclustermain.cpp
 *  @brief
 **/

#include <Rcpp.h>
RcppExport SEXP CoClustmain(SEXP robj)
{
  BEGIN_RCPP
  //Initialize Rcpp object
  Rcpp::S4 CoClustobj(robj);

  return CoClustobj;
  END_RCPP
}
#endif
