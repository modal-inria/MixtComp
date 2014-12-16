/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2014

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 *  Project:    MixtComp
 *  Created on: Nov 21, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <Rcpp.h>

// [[Rcpp::export]]
Rcpp::NumericMatrix fun()
{
  Rcpp::NumericMatrix x(2,2);
  x.fill(42); // or more interesting values
  Rcpp::List dimnms = // two vec. with static names
  Rcpp::List::create(Rcpp::CharacterVector::create("cc", "dd"),
  Rcpp::CharacterVector::create("ee", "ff"));
  // and assign it
  x.attr("dimnames") = dimnms;
  return(x);
}

// [[Rcpp::export]]
Rcpp::NumericMatrix fun2()
{
  Rcpp::NumericMatrix x(2,2);
  x.fill(42); // or more interesting values

  Rcpp::CharacterVector rows(2);
  Rcpp::CharacterVector cols(2);

  rows[0] = "bouga";
  rows[1] = "bougi";

  cols[0] = "tata";
  cols[1] = "toto";

  Rcpp::List dimnms = Rcpp::List::create(rows, cols);

  // and assign it
  x.attr("dimnames") = dimnms;

  return(x);
}
