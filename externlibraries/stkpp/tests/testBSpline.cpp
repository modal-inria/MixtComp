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

/* Project : Regress
 * File    : testBSpline.cpp
 * Contents: test program for the classes BSplineCoefficients and
 * BSplineRegression
 * Author  : Serge Iovleff
*/

#include "../projects/Arrays/include/STK_Array2D.h"
#include "../projects/Arrays/include/STK_Array2DSquare.h"

#include "../projects/DManager/include/STK_ExportToCsv.h"

#include "../projects/Arrays/include/STK_Display.h"


#include "../projects/Regress/include/STK_BSplineCoefficients.h"
#include "../projects/Regress/include/STK_BSplineRegression.h"

#include "../projects/Regress/include/STK_AdditiveBSplineCoefficients.h"
#include "../projects/Regress/include/STK_AdditiveBSplineRegression.h"

#include "../projects/STatistiK/include/STK_Law_Normal.h"
#include "../projects/STatistiK/include/STK_RandBase.h"
#include "../projects/STatistiK/include/STK_Const_Math.h"
#include "../projects/STatistiK/include/STK_Stat_MultivariateMatrix.h"


using namespace STK;

// initialize static random number generator
RandBase Law::ILawBase::generator;


// main
int main(int argc, char *argv[])
{
  int nbControlPoint = 7, degree = 3;
  if (argc > 1)
  {
    nbControlPoint = int(atoi(argv[1]));
  }
  if (argc > 2)
  {
    degree = int(atoi(argv[2]));
  }


  stk_cout << "\n\n";
  stk_cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  stk_cout << "+ test BSplineCoefficients class                    +\n";
  stk_cout << "\n";

  Vector x(Range(1,10), 0.);

  x[1] = 0.0; x[2] = 5.0; x[3] = 0.5; x[4] = 1.0; x[5] = 1.5;
  x[6] = -1.0; x[7] = -5.0; x[8] = -0.5; x[9] = -2.0; x[10] = 3.0;
  
  stk_cout << "x =\n" << x << "\n";

  BSplineCoefficients x_coefs(x, 7, 3);
  x_coefs.run();

  stk_cout << "knots =\n" << x_coefs.knots() << "\n";
  stk_cout << "coefficients =\n" << x_coefs.coefficients() << "\n";

  stk_cout << "\n";
  stk_cout << "+ end of test BSplineCoefficients class              +\n";
  stk_cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";

  stk_cout << "\n\n";
  stk_cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  stk_cout << "+ Simulating 1D manifold                             \n";

  RandBase ran;
  int nb_sample = 150;
  Matrix A(Range(1,nb_sample), Range(1,3));
  x.resize(Range(1,nb_sample));
  Real step = 1./nb_sample;
  // simulate 1D manifold
  for (int i=1; i<=nb_sample; i++)
  {
    Real z = -0.5 + i * step;
    x[i] = z;
    A(i, 1) = sin((double)z*(3.*Const::_PI_))/2. + ran.randGauss(0., 0.1);
    A(i, 2) = cos((double)z*(3.*Const::_PI_))/2. + ran.randGauss(0., 0.1);
    A(i, 3) = z + ran.randGauss(0., 0.1);
  }
  stk_cout << "A simulated : writing it to simul1Dmanifold.csv file\n";
  ExportToCsv data_simul1D(A);
  data_simul1D.p_readWriteCsv()->write("./tests/data/simul1Dmanifold.csv");

  stk_cout << "\n";
  stk_cout << "+ end of Simulating 1D manifold                     +\n";
  stk_cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";

  stk_cout << "\n\n";
  stk_cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  stk_cout << "+ Estimating 1D manifold with uniform knots          \n";

  BSplineRegression a_reg_unif(&A, &x, nbControlPoint, degree, BSplineCoefficients::uniform_);
  a_reg_unif.run();
  // test extrapolate
  Matrix Y, R;
  Y.move(a_reg_unif.extrapolate(x));
  R =  *(a_reg_unif.p_predicted()) - Y;
  Point mu;

  Stat::mean<Matrix>(R, mu);
  stk_cout << "Mean of the predicted values - extrapolated values = \n";
  stk_cout << mu << "\n";
  //
  stk_cout << "knots =\n";
  stk_cout << a_reg_unif.knots() << "\n";

  Stat::MultivariateMatrix statResiduals(a_reg_unif.p_residuals());
  statResiduals.run();
  stk_cout << "Mean of the residuals = \n";
  stk_cout << statResiduals.mean() << "\n";
  stk_cout << "Variance of the residuals = \n";
  stk_cout << statResiduals.variance() << "\n";
  stk_cout << "Covariance of the residuals = \n";
  stk_cout << statResiduals.covariance() << "\n";

  ExportToCsv exporter_unif(A, _T("Var"));
  exporter_unif.append(x, _T("Proj"));
  exporter_unif.append(*(a_reg_unif.p_predicted()), _T("Pred"));
  exporter_unif.append(*(a_reg_unif.p_residuals()), _T("Res"));
  exporter_unif.p_readWriteCsv()->write("./tests/data/recons1Dunif.csv");

  stk_cout << "\n";
  stk_cout << "+ end of Estimating 1D manifold with uniform knots  +\n";
  stk_cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";

  stk_cout << "\n\n";
  stk_cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  stk_cout << "+ Estimating 1D manifold with periodic knots          \n";

  A.shift(0,0);
  x.shift(0);
  BSplineRegression a_reg_period(&A, &x, nbControlPoint, degree, BSplineCoefficients::periodic_);
  a_reg_period.run();
  stk_cout << "knots =\n";
  stk_cout << a_reg_period.knots() << "\n";

  // test extrapolate
  Y.move(a_reg_period.extrapolate(x));
  R = *a_reg_period.p_predicted() - Y;

  Stat::mean<Matrix>(R, mu);
  //
  statResiduals.setData(*(a_reg_period.p_residuals()));
  statResiduals.run();
  stk_cout << "Mean of the predicted values - extrapolated values = \n";
  stk_cout << mu << "\n";
  // test results
  stk_cout << "Mean of the residuals = \n";
  stk_cout << statResiduals.mean() << "\n";
  stk_cout << "Variance of the residuals = \n";
  stk_cout << statResiduals.variance() << "\n";
  stk_cout << "Covariance of the residuals = \n";
  stk_cout << statResiduals.covariance() << "\n";

  ExportToCsv exporter_period(A, _T("Var"));
  exporter_unif.append(x, _T("Proj"));
  exporter_period.append(*(a_reg_unif.p_predicted()), _T("Pred"));
  exporter_period.append(*(a_reg_unif.p_residuals()), _T("Res"));
  exporter_period.p_readWriteCsv()->write("./tests/data/recons1Dperiod.csv");

  stk_cout << "\n";
  stk_cout << "+ end of Estimating 1D manifold with periodic knots +\n";
  stk_cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";

  stk_cout << "\n\n";
  stk_cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  stk_cout << "+ Simulating 2D manifold                             \n";
  nb_sample = 900;
  int nb_step = 30;
  step = 1./nb_step;
  A.resize(Range(1,nb_sample), Range(1,3));
  x.resize(Range(1,nb_sample));
  Vector y(Range(1,nb_sample));
  Matrix B(Range(1,nb_sample),Range(1,2));
  int k =1;
  for (int i=1; i<=nb_step; i++)
    for (int j=1; j<=nb_step; j++)
    {
      Real xr = -0.5 + step*i;
      Real yr = -0.5 + step*j;
      double xd = (double)xr, yd = (double)yr;
      double zd = (cos(Const::_PI_*sqrt(xd*xd + yd*yd))* (1. - exp(-64*(xd*xd + yd*yd))))/ 2.0;
      x[k] = xr;
      y[k] = yr;
      B(k,1) = (A(k, 1) = xr + ran.randGauss(0., 0.1));
      B(k,2) = (A(k, 2) = yr + ran.randGauss(0., 0.1));
      A(k, 3) = Real(zd)+ ran.randGauss(0., 0.1);
      k++;
    }
  stk_cout << "A simulated : writing it to simul2Dmanifold.csv file\n";
  ExportToCsv data_simul2D(A);
  data_simul2D.p_readWriteCsv()->write("./tests/data/simul2Dmanifold.csv");

  stk_cout << "\n";
  stk_cout << "+ end of Simulating 2D manifold                     +\n";
  stk_cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";

   stk_cout << "\n\n";
   stk_cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
   stk_cout << "+ Estimating 2D manifold with uniform knots          \n";

   AdditiveBSplineRegression reg_unif(&A, &B, nbControlPoint, degree, BSplineCoefficients::uniform_);
   reg_unif.run();
   // test extrapolate
   Y.move(reg_unif.extrapolate(B));
   R = (*(reg_unif.p_predicted())) - Y;
   Stat::mean<Matrix>(R, mu);
   stk_cout << "Mean of the predicted values - extrapolated values = \n";
   stk_cout << mu << "\n";

   statResiduals.setData(*(reg_unif.p_residuals()));
   statResiduals.run();
   stk_cout << "Mean of the residuals = \n";
   stk_cout << statResiduals.mean() << "\n";
   stk_cout << "Variance of the residuals = \n";
   stk_cout << statResiduals.variance() << "\n";
   stk_cout << "Covariance of the residuals = \n";
   stk_cout << statResiduals.covariance() << "\n";

   ExportToCsv exporter2D_unif(A, _T("Var"));
   exporter2D_unif.append<Matrix>(B, _T("Proj"));
   exporter2D_unif.append<Matrix>(*(reg_unif.p_predicted()), _T("Pred"));
   exporter2D_unif.append<Matrix>(*(reg_unif.p_residuals()), _T("Res"));
   exporter2D_unif.p_readWriteCsv()->write("./tests/data/recons2Dunif.csv");

   stk_cout << "\n";
   stk_cout << "+ end of Estimating 2D manifold with uniform knots  +\n";
   stk_cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";

   stk_cout << "\n\n";
   stk_cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
   stk_cout << "+ Estimating 2D manifold with periodic knots         \n";
   A.shift(0,0);
   B.shift(0,0);
   AdditiveBSplineRegression reg_period(&A, &B, nbControlPoint, degree, BSplineCoefficients::periodic_);
   reg_period.run();
   // test extrapolate
   Y.move(reg_period.extrapolate(B));
   R = (*(reg_period.p_predicted())) - Y;

   Stat::mean<Matrix>(R, mu);
   stk_cout << "Mean of the predicted values - extrapolated values = \n";
   stk_cout << mu << "\n";

   // test results
   statResiduals.setData(*(reg_period.p_residuals()));
   statResiduals.run();
   stk_cout << "Mean of the residuals = \n";
   stk_cout << statResiduals.mean() << "\n";
   stk_cout << "Variance of the residuals = \n";
   stk_cout << statResiduals.variance() << "\n";
   stk_cout << "Covariance of the residuals = \n";
   stk_cout << statResiduals.covariance() << "\n";

   ExportToCsv exporter2D_period(A, _T("Var"));
   exporter2D_unif.append<Matrix>(B, _T("Proj"));
   exporter2D_unif.append<Matrix>(*(reg_period.p_predicted()), _T("Pred"));
   exporter2D_unif.append<Matrix>(*(reg_period.p_residuals()), _T("Res"));
   exporter2D_unif.p_readWriteCsv()->write("./tests/data/recons2Dperiod.csv");

   stk_cout << "\n";
   stk_cout << "+ end of Estimating 2D manifold with periodic knots +\n";
   stk_cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  stk_cout << "\n\n";
  stk_cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  stk_cout << "+ Successful completion of testing for TestBSpline. +\n";
  stk_cout << "+ No errors detected.                               +\n";
  stk_cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  stk_cout << "\n\n";

  return 0;
}
