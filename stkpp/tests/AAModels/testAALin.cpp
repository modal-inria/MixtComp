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
  
    Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)                                   */

/* Project : AAModels
 * File    : testAALin.cpp
 * Contents: test program for the class AALinModel
 * Author  : Serge Iovleff
*/

//#include "AAModels/include/STK_LinearAAModel.h"
//#include "Reduct/include/STK_LocalVariance.h"
//
//#include "Analysis/include/STK_Const_Math.h"
//
//#include "STatistiK/include/STK_Law_Cauchy.h"
//#include "STatistiK/include/STK_Law_Normal.h"
//#include "STatistiK/include/STK_Stat_MultivariateReal.h"
//
//#include "DManager/include/STK_ExportToCsv.h"
#include "AAModels.h"
#include "Reduct.h"
#include "STatistiK.h"


using namespace STK;

/* main print */
template< class TYPE, template<class> class Container2D >
void printHead(Container2D<TYPE> const& A, String const& name)
{
  stk_cout << "print: " << name << _T("\n";);
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
  stk_cout << name << _T("=\n")               << A << _T(_T("\n\n"));
}


/* main. */
int main(int argc, char *argv[])
{
//  try
//  {
  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";)
  stk_cout << _T("+ TestAALin                                         +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("\n\n");

  // first test
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Test LocalVariance class (on a small data set)    +\n");
  Matrix A(Range(1,5),Range(1,2));
  
  A(1,1) = 0.0; A(1,2) = 0.0;
  A(2,1) = 5.0; A(2,2) = 5.0;
  A(3,1) = -5.0; A(3,2) = 5.0;
  A(4,1) = 1.0; A(4,2) = 1.0;
  A(5,1) = -1.5; A(5,2) = 1.5;

  //A.shift(0,0);
  stk_cout << _T("A =\n") << A << _T("\n");

  LocalVariance Ind(A, LocalVariance::distance_, 2);
  Ind.setDimension(2);
  Ind.run();
  stk_cout << _T("pred = \n") << Ind.pred() << _T("\n";);
  stk_cout << _T("covariance =\n") << Ind.covariance() << _T("\n";);
  stk_cout << _T("local covariance =\n") << Ind.localCovariance() << _T("\n";);
  stk_cout << _T("criteria Values = ") << Ind.criteriaValues() << _T("\n";);
  stk_cout << _T("Axis =\n") << Ind.axis() << _T("\n";);

  stk_cout << "+ End of Test LocalVariance class (small data set)  +\n";
  stk_cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";

  stk_cout << _T("\n\n");
  stk_cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  stk_cout << "+ Test LinearAAModel class.                          \n";
  LinearAAModel test1(A);
  test1.center();
  test1.setReductor(&Ind);
  test1.run(1);
  stk_cout << "restoredData = \n" << *(test1.p_predicted()) << _T("\n";);
  stk_cout << "residuals = \n" << *(test1.p_residuals()) << _T("\n";);
  //  stk_cout << "mean = \n" << test1.mean() << _T("\n";);
  Point statistics;
  statistics = Stat::variance(*(test1.p_residuals()));
  stk_cout << "residualVariance = \n" << statistics << _T("\n";);

  stk_cout << "\nTest LinearAAModel class with weights (small data set)\n";
  Vector weight1(Range(1,5), 0.2);
  weight1.shift(0);
  test1.run(weight1, 1);
  stk_cout << "restoredData = \n" << *(test1.p_predicted()) << _T("\n";);
  stk_cout << "residuals = \n" << *(test1.p_residuals()) << _T("\n";);

  stk_cout << "mean = \n" << test1.mean() << _T("\n";);
  statistics = Stat::variance(*(test1.p_residuals()));
  stk_cout << "residualVariance = \n" << statistics << _T("\n";);

  stk_cout << "+End of Test LinearAAModel class (small data set)   +\n";
  stk_cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";


  // second test
  stk_cout << "Test LocalVariance class (simulated data set)\n";
  stk_cout << "Simulated clusters\n";
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
  stk_cout << "A simulated : writing it to simul2manifold.csv file\n";

  ExportToCsv csv_file(A);
  csv_file.p_readWriteCsv()->write("./tests/data/simul2manifold.csv");

  // first test of the LinearAAModel class
  stk_cout << "Test LinearAAmodel class without weights\n";

  LinearAAModel test(A);
  test.center();
  LocalVariance* testlocal = new LocalVariance(A, LocalVariance::distance_, 2);
  test.setReductor(testlocal);
  test.run(2);

  stk_cout << "cov =\n" << testlocal->covariance() << _T("\n";);
  stk_cout << "local cov =\n" << testlocal->localCovariance() << _T("\n";);
  stk_cout << "Axis =\n" << testlocal->axis() << _T("\n";);
  stk_cout << "Index values =\n" << testlocal->criteriaValues() << _T("\n";);

  stk_cout << "Writing proximity graph to pred2manifold.csv file\n";
  ExportToCsv csv_pred_file(testlocal->pred());
  csv_pred_file.p_readWriteCsv()->write("./tests/data/pred2manifold.csv");

  stk_cout << "Writing constructed data set to estim2manifold.csv file\n";
  ExportToCsv csv_estim_file(*(test.p_predicted()));
  csv_estim_file.p_readWriteCsv()->write("./tests/data/estim2manifold.csv");

  // second test of the LinearAAModel class with weights
  stk_cout << "Test LinearAAmodel class with weights\n";
  Vector weights(Range(1,300), 1.0);
  weights.sub(Range(101,200)) *= 100.0;
  weights /= 20100.0;
  //stk_cout << _T("Weights =\n") << weights << _T("\n");

  LinearAAModel wtest(A);
  wtest.setReductor(testlocal);
  wtest.run(weights, 2);

  stk_cout << "results with second manifold weighted\n";
  stk_cout << "cov =\n" << ((LocalVariance*)testlocal)->covariance() << _T("\n";);
  stk_cout << "local cov =\n" << ((LocalVariance*)testlocal)->localCovariance() << _T("\n";);
  stk_cout << "Axis =\n" << testlocal->axis() << _T("\n";);
  stk_cout << "Index values =\n" << testlocal->criteriaValues() << _T("\n";);

  stk_cout << "Writing constructed data set to westim2manifold.csv file\n";
  ExportToCsv csv_westim2_file(*(wtest.p_predicted()));
  csv_westim2_file.p_readWriteCsv()->write("./tests/data/westim2manifold.csv");

  weights = 1.0;
  weights.sub(Range(1,100, 0)) *=100.0;
  weights /= 10200.0;

//  wtest.wcenter(&weights);
  wtest.run(weights, 1);
  stk_cout << "results with first manifold weighted\n";
  stk_cout << "cov =\n" << ((LocalVariance*)testlocal)->covariance() << _T("\n";);
  stk_cout << "local cov =\n" << ((LocalVariance*)testlocal)->localCovariance() << _T("\n";);
  stk_cout << "Axis =\n" << testlocal->axis() << _T("\n";);
  stk_cout << "Index values =\n" << testlocal->criteriaValues() << _T("\n";);
  stk_cout << "Projected covariance =\n" << wtest.projectedCovariance() << _T("\n";);
  stk_cout << "Residual covariance =\n" << wtest.residualCovariance() << _T("\n";);

  delete testlocal;

  stk_cout << "Writing constructed data set to westim1manifold.csv file\n";
  ExportToCsv csv_westim1_file(*(wtest.p_predicted()));
  csv_westim1_file.p_readWriteCsv()->write("./tests/data/westim1manifold.csv");
  stk_cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";

  stk_cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  stk_cout << "Test LinearAAmodel simulation method\n";

  Law::Normal law(0.0, 4.0);
  Matrix proj(Range(1,3), Range(1,2));
  Vector mean(Range(1,3));
  law.randArray(mean);
  stk_cout << "mean =" << mean;
  stk_cout << "std = 1.0\n";
  LinearAAModel::simul(law, mean, 1.0, proj, A);

  stk_cout << "Writing simulated data set to simul2.csv file\n";
  ExportToCsv csv_file2(A);
  csv_file2.p_readWriteCsv()->write("./tests/data/simul2.csv");
  stk_cout << "+ End of simulation test. \n";
  stk_cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";

  stk_cout << _T("\n\n");
  stk_cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  stk_cout << "+   Successful completion of testing for TestAALin  +\n";
  stk_cout << "+   No errors detected.                             +\n";
  stk_cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  stk_cout << _T("\n\n");
//  }
//  catch (Exception const& error)
//  {
//    std::cerr << error.error() << _T("\n";);
//  }

  return 0;
}
