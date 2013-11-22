#include <iostream>
#include <Rcpp.h>

// [[Rcpp::export]]
void rcpp_change_var(Rcpp::NumericVector a)
{
  a[3] = 999.;
}

// example from http://stackoverflow.com/questions/11300048/rcpp-pass-by-reference-vs-by-value
// [[Rcpp::export]]
Rcpp::NumericVector cpp_if(Rcpp::NumericVector a)
{
  Rcpp::NumericVector xa(a);
  int n_xa = xa.size();
  for(int i=0; i < n_xa; i++)
  {
    if(xa[i]<0) xa[i] = 0;
  }
  return xa;
}

// [[Rcpp::export]]
void cpp_if_2(Rcpp::NumericVector a)
{
  Rcpp::NumericVector xa(a);
  int n_xa = xa.size();
  for(int i=0; i < n_xa; i++)
  {
    if(xa[i]<0) xa[i] = 0;
  }
}

// [[Rcpp::export]]
void cpp_if_3(Rcpp::NumericVector a)
{
  int n_a = a.size();
  for(int i=0; i < n_a; i++)
  {
    if(a[i]<0) a[i] = 0;
  }
}

// [[Rcpp::export]]
Rcpp::NumericVector cpp_if_4(Rcpp::NumericVector a)
{
  Rcpp::NumericVector xa(a);
  int n_xa = xa.size();
  Rcpp::NumericVector xr(a);
  for(int i=0; i < n_xa; i++) {
    if(xr[i]<0) xr[i] = 0;
  }
  return xr;
}