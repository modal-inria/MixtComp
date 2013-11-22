#include <iostream>
#include <Rcpp.h>

// [[Rcpp::export]]
void updateS4ByRef(Rcpp::List listS4)
{
  Rcpp::S4 objS4 = listS4[0]; // const& not necessary, as object is a reference by nature
  Rcpp::NumericMatrix nm = objS4.slot("data");
  std::string model = objS4.slot("model");
  std::cout << model << std::endl;
  std::cout << nm.nrow() << std::endl;
  std::cout << nm.ncol() << std::endl;
  std::cout << nm.begin() << std::endl;
  std::cout << nm[2] << std::endl;
  double* p_nm = nm.begin();
  p_nm[1] = 99.;
  nm[1] = 88.;
}

// [[Rcpp::export]]
void updateDFByRef(Rcpp::DataFrame DF) // passing parameter by reference is useless
{
    Rcpp::NumericVector a = DF["age"];
    Rcpp::NumericVector b = DF(1);
    std::cout << a[3] << std::endl;
    std::cout << b[3] << std::endl;
    std::cout << a.begin() << std::endl;
    a[3] = 888.;
    b[3] = 999.;
    double* p_a = a.begin();
    p_a[1] = 213.;
}

// [[Rcpp::export]]
Rcpp::List addColDF(Rcpp::DataFrame df, std::string name)
{
  int nr = df.nrows(), nc= df.size() ;
  Rcpp::NumericVector newCol(nr,1.);
  Rcpp::List out(nc+1) ;
  Rcpp::CharacterVector onames = df.attr("names") ;
  Rcpp::CharacterVector names( nc + 1 ) ;
  for( int i=0; i<nc; i++)
  {
    out[i] = df[i] ;
    names[i] = onames[i] ;
  }
  
  out[nc] = newCol ;
  names[nc] = name ;
  out.attr("class") = df.attr("class") ;
  out.attr("row.names") = df.attr("row.names") ;
  out.attr("names") = names ;
  return out ;
}