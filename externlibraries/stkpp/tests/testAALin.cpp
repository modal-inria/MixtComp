/*--------------------------------------------------------------------*/
/*     Copyright (C) 2003  Serge Iovleff

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program; if not, write to the
    Free Software Foundation, Inc.,
    59 Temple Place,
    Suite 330,
    Boston, MA 02111-1307
    USA
  
    Contact : Serge.Iovleff@stkpp.org                                   */

/* Project : AAModels
 * File    : testAALin.cpp
 * Contents: test program for the class AALinModel
 * Author  : Serge Iovleff
*/

#include "../projects/AAModels/include/STK_LinearAAModel.h"
#include "../projects/Reduct/include/STK_LocalVariance.h"

#include "../projects/STatistiK/include/STK_Law_Cauchy.h"
#include "../projects/STatistiK/include/STK_Law_Normal.h"

#include "../projects/STatistiK/include/STK_Const_Math.h"
#include "../projects/STatistiK/include/STK_Stat_MultivariateMatrix.h"

#include "../projects/DManager/include/STK_ExportToCsv.h"
#include "../include/STKpp.h"


using namespace STK;

// initialize static member generator
RandBase Law::ILawBase::generator;


/* main print */
/* main print */
template< class TYPE, template<class> class Container2D >
void printHead(Container2D<TYPE> const& A, String const& name)
{
  stk_cout << "print: " << name << "\n";
  stk_cout << name << _T(".isRef() =")        << A.isRef()  << _T("\n");
  stk_cout << name << _T(".capacityHo() =")   << A.capacityHo()  << _T("\n");
  stk_cout << name << _T(".cols() =")      << A.cols()  << _T(" | ");
  stk_cout << name << _T(".firstIdxCols() =")   << A.firstIdxCols()  << _T(" ");
  stk_cout << name << _T(".lastIdxCols() =")      << A.lastIdxCols()  << _T(" ");
  stk_cout << name << _T(".sizeCols() =")      << A.sizeCols()  << _T("\n");
  stk_cout << name << _T(".rows() =")      << A.rows()  << _T(" | ");
  stk_cout << name << _T(".firstIdxRows() =")   << A.firstIdxRows()  << _T(" ");
  stk_cout << name << _T(".lastIdxRows() =")      << A.lastIdxRows()  << _T(" ");
  stk_cout << name << _T(".sizeRows() =")      << A.sizeRows()  << _T("\n");
  stk_cout << name << _T(".rangeCols().isRef() =")  << A.rangeCols().isRef() << _T("\n");
  stk_cout << name << _T(".rangeCols() =\n")  << A.rangeCols() << _T("\n");
  stk_cout << name << _T(".capacityCols().isRef() =") << A.capacityCols().isRef()  << _T("\n");
  stk_cout << name << _T(".capacityCols() =\n") << A.capacityCols()  << _T("\n");
}

template< class TYPE, template<class> class Container2D >
void print(Container2D<TYPE> const& A, String const& name)
{
  printHead(A, name);
  stk_cout << name << _T("=\n")               << A << _T("\n\n");
}


/* main.                                                              */
int main(int argc, char *argv[])
{
//  try
//  {
  std::cout << "\n\n";
  std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  std::cout << "+ TestAALin                                       +\n";
  std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  std::cout << "\n\n";

  // first test
  std::cout << "\n\n";
  std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  std::cout << "+ Test LocalVariance class (on a small data set)    +\n";
  Matrix A(Range(1,5),Range(1,2));
  
  A(1,1) = 0.0; A(1,2) = 0.0;
  A(2,1) = 5.0; A(2,2) = 5.0;
  A(3,1) = 0.0; A(3,2) = 1.0;
  A(4,1) = 1.0; A(4,2) = 1.0;
  A(5,1) = 1.5; A(5,2) = 1.5;

  A.shift(0,0);
  std::cout << "A =\n";
  std::cout << A << "\n";

  LocalVariance Ind(A, LocalVariance::minimalDistance_, 2);
  Ind.setDimension(1);
  Ind.run();
  std::cout << "pred = \n" << Ind.pred() << "\n";
  std::cout << "cov =\n" << Ind.covariance() << "\n";
  std::cout << "local cov =\n" << Ind.localCovariance() << "\n";
  std::cout << "+ End of Test LocalVariance class (small data set)  +\n";
  std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";

  std::cout << "\n\n";
  std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  std::cout << "+ Test LinearAAModel class.                          \n";
  LinearAAModel test1(A);
  test1.center();
  test1.setReductor(&Ind);
  test1.run(1);
  std::cout << "restoredData = \n" << *(test1.p_predicted()) << "\n";
  std::cout << "residuals = \n" << *(test1.p_residuals()) << "\n";
  //  std::cout << "mean = \n" << test1.mean() << "\n";
  Point statistics;
  Stat::variance(*(test1.p_residuals()), statistics);
  std::cout << "residualVariance = \n" << statistics << "\n";

  std::cout << "\nTest LinearAAModel class with weights (small data set)\n";
  Vector weight1(Range(1,5), 0.2);
  weight1.shift(0);
  test1.run(weight1, 1);
  std::cout << "restoredData = \n" << *(test1.p_predicted()) << "\n";
  std::cout << "residuals = \n" << *(test1.p_residuals()) << "\n";

  std::cout << "mean = \n" << test1.mean() << "\n";
  Stat::variance(*(test1.p_residuals()), statistics);
  std::cout << "residualVariance = \n" << statistics << "\n";

  std::cout << "+End of Test LinearAAModel class (small data set)   +\n";
  std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";


  // second test
  std::cout << "Test LocalVariance class (simulated data set)\n";
  std::cout << "Simulated clusters\n";
  RandBase ran;
  A.resize(Range(1,300), Range(1,3));
  // simulate 1D manifold
  for (int i=1; i<=100; i++)
  {
    Real z = 8.* ran.randUnif() -4.;
    A(i, 1) = sin((double)z*(4.*Const::_PI_))/2. + ran.randGauss(0., 0.1);
    A(i, 2) = cos((double)z*(4.*Const::_PI_))/2. + ran.randGauss(0., 0.1);
    A(i, 3) = z + ran.randGauss(0., 0.1);
  }
  // simulate 2D manifold
  for (int i=101; i<=300; i++)
  {
    Real x = 2.* ran.randUnif() - 1.;
    Real y = 2 * ran.randUnif() - 1;
    double xd = (double)x, yd = (double)y;
    A(i, 3) = (cos(Const::_PI_*sqrt(xd*xd + yd*yd))* (1. - exp(-64*(xd*xd + yd*yd))))
               / 2.0
            + ran.randGauss(0., 0.1);
    A(i, 1) = x + ran.randGauss(0., 0.1);
    A(i, 2) = y + ran.randGauss(0., 0.1);
  }
  std::cout << "A simulated : writing it to simul2manifold.csv file\n";

  ExportToCsv csv_file(A);
  csv_file.p_readWriteCsv()->write("./tests/data/simul2manifold.csv");

  // first test of the LinearAAModel class
  std::cout << "Test LinearAAmodel class without weights\n";

  LinearAAModel test(A);
  test.center();
  LocalVariance* testlocal = new LocalVariance(A, LocalVariance::minimalDistance_, 2);
  test.setReductor(testlocal);
  test.run(2);

  std::cout << "cov =\n" << testlocal->covariance() << "\n";
  std::cout << "local cov =\n" << testlocal->localCovariance() << "\n";
  std::cout << "Axis =\n" << testlocal->axis() << "\n";
  std::cout << "Index values =\n" << testlocal->criteriaValues() << "\n";

  std::cout << "Writing proximity graph to pred2manifold.csv file\n";
  ExportToCsv csv_pred_file(testlocal->pred());
  csv_pred_file.p_readWriteCsv()->write("./tests/data/pred2manifold.csv");

  std::cout << "Writing constructed data set to estim2manifold.csv file\n";
  ExportToCsv csv_estim_file(*(test.p_predicted()));
  csv_estim_file.p_readWriteCsv()->write("./tests/data/estim2manifold.csv");

  // second test of the LinearAAModel class with weights
  std::cout << "Test LinearAAmodel class with weights\n";
  Vector weights(Range(1,300), 1.0);
  weights.sub(Range(101,200)) *= 100.0;
  weights /= 20100.0;
  //stk_cout << _T("Weights =\n") << weights << _T("\n");

  LinearAAModel wtest(A);
  wtest.setReductor(testlocal);
  wtest.run(weights, 2);

  std::cout << "results with second manifold weighted\n";
  std::cout << "cov =\n" << ((LocalVariance*)testlocal)->covariance() << "\n";
  std::cout << "local cov =\n" << ((LocalVariance*)testlocal)->localCovariance() << "\n";
  std::cout << "Axis =\n" << testlocal->axis() << "\n";
  std::cout << "Index values =\n" << testlocal->criteriaValues() << "\n";

  std::cout << "Writing constructed data set to westim2manifold.csv file\n";
  ExportToCsv csv_westim2_file(*(wtest.p_predicted()));
  csv_westim2_file.p_readWriteCsv()->write("./tests/data/westim2manifold.csv");

  weights = 1.0;
  weights.sub(Range(1,100, 0)) *=100.0;
  weights /= 10200.0;

//  wtest.wcenter(&weights);
  wtest.run(weights, 1);
  std::cout << "results with first manifold weighted\n";
  std::cout << "cov =\n" << ((LocalVariance*)testlocal)->covariance() << "\n";
  std::cout << "local cov =\n" << ((LocalVariance*)testlocal)->localCovariance() << "\n";
  std::cout << "Axis =\n" << testlocal->axis() << "\n";
  std::cout << "Index values =\n" << testlocal->criteriaValues() << "\n";
  std::cout << "Projected covariance =\n" << wtest.projectedCovariance() << "\n";
  std::cout << "Residual covariance =\n" << wtest.residualCovariance() << "\n";

  delete testlocal;

  std::cout << "Writing constructed data set to westim1manifold.csv file\n";
  ExportToCsv csv_westim1_file(*(wtest.p_predicted()));
  csv_westim1_file.p_readWriteCsv()->write("./tests/data/westim1manifold.csv");
  std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";

  std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  std::cout << "Test LinearAAmodel simulation method\n";

  Law::Normal law(0.0, 4.0);
  Matrix proj(Range(1,3), Range(1,2));
  Vector mean(Range(1,3));
  law.randArray(mean);
  std::cout << "mean =" << mean;
  std::cout << "std = 1.0\n";
  LinearAAModel::simul(law, mean, 1.0, proj, A);

  std::cout << "Writing simulated data set to simul2.csv file\n";
  ExportToCsv csv_file2(A);
  csv_file2.p_readWriteCsv()->write("./tests/data/simul2.csv");
  std::cout << "+ End of simulation test. \n";
  std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";

  std::cout << "\n\n";
  std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  std::cout << "+   Successful completion of testing for TestAALin  +\n";
  std::cout << "+   No errors detected.                             +\n";
  std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  std::cout << "\n\n";
//  }
//  catch (Exception const& error)
//  {
//    std::cerr << error.error() << "\n";
//  }

  return 0;
}
