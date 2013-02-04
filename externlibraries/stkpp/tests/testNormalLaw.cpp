/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2011  Serge Iovleff

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this program; if not, write to the
    Free Software Foundation, Inc.,
    59 Temple Place,
    Suite 330,
    Boston, MA 02111-1307
    USA

    Contact : Serge.Iovleff@stkpp.org
*/

/*
 * Project:  stkpp::tests
 * created on: 8 août 2011
 * Purpose:  test the Normal and MultiNormal classes.
 * Author:   iovleff, serge.iovleff@stkpp.org
 *
 **/

/** @file testNormalLaw.cpp
 *  @brief In this file we test the Normal and MultiNormal classes.
 **/

#include "../include/STKpp.h"

using namespace STK;

// initialize static member generator
RandBase Law::ILawBase::generator;

/* main. */
int main(int argc, char *argv[])
{
  int N;
  N= (argc < 2) ? 20 : int(atoi(argv[1]));

  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Test Normal Law                                   +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("\n\n");

  // first test. Law with a mean (1,0,1) and S = Diag(1, 2, 1)
  Point mu(Range(1,3), 1.); mu[2] = 0.0;
  MatrixSquare sigma(Range(1,3), 0.);
  sigma(1,1) = 1.; sigma(2,2) = 2.; sigma(3,3) = 1.;


  stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Creating Multivariate Normal law with diagonal covariance matrix +\n");
  stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

  Law::MultiNormal<Point> multilaw(mu, sigma);

  stk_cout << _T("mu =\n") << multilaw.mu() << _T("\n");
  stk_cout << _T("sigma =\n") << multilaw.sigma() << _T("\n");
  stk_cout << _T("eigenvalues =\n") << multilaw.decomp().eigenvalues() << _T("\n");
  stk_cout << _T("rotation =\n") << multilaw.decomp().rotation() << _T("\n\n");

  Point zero(Range(1,3), 1.); zero[2] = 0.0;
  stk_cout << _T("\nCompute pdf of (1, 0, 1) = ") << multilaw.pdf(zero) << "\n";
  stk_cout << _T("Compare with 1./sqrt(2pi)^3 * 1/sqrt(2) = ")
           << Const::_1_SQRT2PI_*Const::_1_SQRT2PI_*Const::_1_SQRT2PI_*Const::_1_SQRT2_;
  stk_cout << _T("\n\n");

  zero[2] = 1.0;
  stk_cout << _T("\nCompute pdf of (1, 1, 1) = ") << multilaw.pdf(zero) << "\n";
  stk_cout << _T("Compare with 1./sqrt(2pi)^3 * 1/sqrt(2) * exp(-0.25) = ")
           << Const::_1_SQRT2PI_*Const::_1_SQRT2PI_*Const::_1_SQRT2PI_*Const::_1_SQRT2_ * exp(-0.25);
  stk_cout << _T("\n\n");

  // simulate law object
  stk_cout << _T("Simulate Data set with n=" << N << " rows\n");
  Matrix data(Range(1,N), Range(1,3));
  multilaw.rand(data);

//#ifdef STK_DEBUG
//stk_cout << _T("data =\n") << data;
//#endif
  // compute lnLikelihood
  stk_cout << _T("\nCompute ln-likelihood using the theoretical parameters :\n");
  Real lnliklihood = diagonalGaussianLnLikelihood(data, mu, sigma);

  stk_cout << _T("ln-likelihood using function =" << lnliklihood << "\n");
  stk_cout << _T("ln-likelihood using law method =" << multilaw.lnLikelihood(data) << "\n\n");

  stk_cout << _T("Compute multivariate Statistics\n");
  Stat::MultivariateMatrix multistat(&data);
  multistat.run();
  stk_cout << _T("mean =\n") << multistat.mean() << _T("\n");
  stk_cout << _T("covariance =\n") << multistat.covariance() << _T("\n");

  stk_cout << _T("Create law with the computed multivariate Statistics\n");
  Law::MultiNormal<Point> multilaw2(multistat.mean(), multistat.covariance());
  stk_cout << _T("ln-likelihood using law method =" << multilaw2.lnLikelihood(data) << "\n\n");

  stk_cout << _T("Compute Gaussian Statistical Model\n");
  GaussianModel statmodel(&data);
  statmodel.run();

  stk_cout << _T("nbSample = ") << statmodel.nbSample() << _T("\n");
  stk_cout << _T("nbVar = ") << statmodel.nbVar() << _T("\n");
  stk_cout << _T("mean =\n") << statmodel.mean() << _T("\n");
  stk_cout << _T("covariance =\n") << statmodel.covariance() << _T("\n");

  stk_cout << _T("lnNbSample = ") << statmodel.lnNbSample() << _T("\n");
  stk_cout << _T("nbFreeParameter = ") << statmodel.nbFreeParameter() << _T("\n");

  stk_cout << _T("likelihood = ") << statmodel.likelihood() << _T("\n");
  stk_cout << _T("lnLikelihood = ") << statmodel.lnLikelihood() << _T("\n");

  stk_cout << _T("\nCompute pdf for 5 observations\n");
  for (int i = data.firstIdxRows(); i <= std::min(data.lastIdxRows(), int(5)); ++i)
  {
    stk_cout << "data(" << i << ")=" << data.row(i) << "\n";
    stk_cout << "empirical pdf(data(" << i << "))=" << statmodel.p_law()->pdf(data.row(i)) << "\n";
    stk_cout << "empirical lpdf(data(" << i << "))=" << statmodel.p_law()->lpdf(data.row(i)) << "\n";
    stk_cout << "theoretical pdf(data(" << i << "))=" << multilaw.pdf(data.row(i)) << "\n";
    stk_cout << "theoretical lpdf(data(" << i << "))=" << multilaw.lpdf(data.row(i)) << "\n";
  }

  stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Testing simulation method with 1 based arrays and 0 based arrays +\n");
  stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  sigma(1,2) = 0.5; sigma(2,1) = 0.5;
  sigma(2,3) = 0.5; sigma(3,2) = 0.5;

  stk_cout << _T("Results with 1 based arrays:\n");
  multilaw.setParameters(mu, sigma);
  stk_cout << _T("mu =\n") << multilaw.mu() << _T("\n");
  stk_cout << _T("sigma =\n") << multilaw.sigma() << _T("\n");
  stk_cout << _T("squareroot =\n") << multilaw.squareroot() << _T("\n");
  stk_cout << _T("\n\n");

  // simulate law object
  stk_cout << _T("Simulate bigData(Range(1,10000), Range(1,3)\n");
  Matrix bigData(Range(1,10000), Range(1,3));
  multilaw.rand(bigData);

  stk_cout << _T("Compute bigData multivariate Statistics\n");
  multistat.setData(bigData);
  multistat.run();
  stk_cout << _T("mean =\n") << multistat.mean() << _T("\n");
  stk_cout << _T("covariance =\n") << multistat.covariance() << _T("\n");
  stk_cout << _T("\n\n");

  stk_cout << _T("shift bigData\n");
  bigData.shift(0, 0);
  stk_cout << _T("Compute bigData multivariate Statistics\n");
  multistat.setData(bigData);
  multistat.run();
  stk_cout << _T("mean =\n") << multistat.mean() << _T("\n");
  stk_cout << _T("covariance =\n") << multistat.covariance() << _T("\n");
  stk_cout << _T("\n\n");

  stk_cout << _T("shift mu and sigma:\n");
  mu.shift(0);
  sigma.shift(0);
  stk_cout << _T("sigma.range() =\n") << sigma.range() << _T("\n");
  multilaw.setParameters(mu, sigma);
  stk_cout << _T("mu =\n") << multilaw.mu() << _T("\n");
  stk_cout << _T("sigma =\n") << multilaw.sigma() << _T("\n");
  stk_cout << _T("sigma.range() =\n") << multilaw.sigma().range() << _T("\n");
  stk_cout << _T("squareroot =\n") << multilaw.squareroot() << _T("\n");
  stk_cout << _T("squareroot.range() =\n") << multilaw.squareroot().range() << _T("\n");
  stk_cout << _T("\n\n");

  // simulate law object
  stk_cout << _T("Simulate bigData0(Range(0,10000), Range(0,3)\n");
  Matrix bigData0(Range(0,10000), Range(0,3));
  multilaw.rand(bigData0);
  stk_cout << _T("Compute bigData multivariate Statistics\n");
  multistat.setData(bigData0);
  multistat.run();
  stk_cout << _T("mean =\n") << multistat.mean() << _T("\n");
  stk_cout << _T("covariance =\n") << multistat.covariance() << _T("\n");
  stk_cout << _T("\n\n");

  stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Creating Multivariate Normal law with general covariance matrix  +\n");
  stk_cout << _T("+ and data, mu and sigma shifted to zero                           +\n");
  stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("Simulate Data set with n=" << N << " rows\n");
  data.shift(0,0);
  multilaw.rand(data);
  multistat.setData(data);
  multistat.run();
  stk_cout << _T("mean =\n") << multistat.mean() << _T("\n");
  stk_cout << _T("covariance =\n") << multistat.covariance() << _T("\n");
  stk_cout << _T("\n\n");

  stk_cout << _T("Compute Gaussian Statistical Model\n");
  statmodel.setData(data);
  statmodel.run();
  stk_cout << _T("nbSample = ") << statmodel.nbSample() << _T("\n");
  stk_cout << _T("nbVar = ") << statmodel.nbVar() << _T("\n");
  stk_cout << _T("lnNbSample = ") << statmodel.lnNbSample() << _T("\n");
  stk_cout << _T("nbFreeParameter = ") << statmodel.nbFreeParameter() << _T("\n");
  stk_cout << _T("likelihood = ") << statmodel.likelihood() << _T("\n");
  stk_cout << _T("lnLikelihood = ") << statmodel.lnLikelihood() << _T("\n");
  stk_cout << _T("mean =\n") << statmodel.mean() << _T("\n");
  stk_cout << _T("covariance =\n") << statmodel.covariance() << _T("\n");
  stk_cout << _T("mu =\n") << static_cast<Law::MultiNormal<Point>const*>(statmodel.p_law())->mu() << _T("\n");
  stk_cout << _T("sigma =\n") << static_cast<Law::MultiNormal<Point>const*>(statmodel.p_law())->sigma() << _T("\n");
  stk_cout << _T("\n\n");

  stk_cout << _T("\nCompute pdf for 5 observations\n\n");
  for (int i = data.firstIdxRows(); i <= std::min(data.lastIdxRows(), int(5)); ++i)
  {
    stk_cout << "data.row(" << i << ")=" << data.row(i);
    stk_cout << "empirical pdf(data.row(" << i << "))=" << statmodel.p_law()->pdf(data.row(i)) << "\n";
    stk_cout << "empirical lpdf(data.row(" << i << "))=" << statmodel.p_law()->lpdf(data.row(i)) << "\n";
    stk_cout << "theoretical pdf(data.row(" << i << "))=" << multilaw.pdf(data.row(i)) << "\n";
    stk_cout << "theoretical lpdf(data.row(" << i << "))=" << multilaw.lpdf(data.row(i)) << "\n\n";
  }
  stk_cout << _T("\n\n");

  stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Compute 2BlocksGaussian Statistical Model                        +\n");
  stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("\n\n");
  Gaussian2BlocksModel stat2model(&data);
  stat2model.run(3);
  stk_cout << _T("Run(3)\n");
  stk_cout << _T("++++++\n");
  stk_cout << _T("nbSample = ") << stat2model.nbSample() << _T("\n");
  stk_cout << _T("nbVar = ") << stat2model.nbVar() << _T("\n");
  stk_cout << _T("lnNbSample = ") << stat2model.lnNbSample() << _T("\n");
  stk_cout << _T("nbFreeParameter = ") << stat2model.nbFreeParameter() << _T("\n");
  stk_cout << _T("likelihood = ") << stat2model.likelihood() << _T("\n");
  stk_cout << _T("lnLikelihood = ") << stat2model.lnLikelihood() << _T("\n");
  stk_cout << _T("mean =\n") << stat2model.mean() << _T("\n");
  stk_cout << _T("covariance =\n") << stat2model.covariance() << _T("\n");
  stk_cout << _T("\n\n");

  stat2model.run(2);
  stk_cout << _T("Run(2)\n");
  stk_cout << _T("++++++\n");
  stk_cout << _T("nbSample = ") << stat2model.nbSample() << _T("\n");
  stk_cout << _T("nbVar = ") << stat2model.nbVar() << _T("\n");
  stk_cout << _T("lnNbSample = ") << stat2model.lnNbSample() << _T("\n");
  stk_cout << _T("nbFreeParameter = ") << stat2model.nbFreeParameter() << _T("\n");
  stk_cout << _T("likelihood = ") << stat2model.likelihood() << _T("\n");
  stk_cout << _T("lnLikelihood = ") << stat2model.lnLikelihood() << _T("\n");
  stk_cout << _T("mean =\n") << stat2model.mean() << _T("\n");
  stk_cout << _T("covariance =\n") << stat2model.covariance() << _T("\n");
  stk_cout << _T("\n\n");

  stat2model.run(1);
  stk_cout << _T("Run(1)\n");
  stk_cout << _T("++++++\n");
  stk_cout << _T("nbSample = ") << stat2model.nbSample() << _T("\n");
  stk_cout << _T("nbVar = ") << stat2model.nbVar() << _T("\n");
  stk_cout << _T("lnNbSample = ") << stat2model.lnNbSample() << _T("\n");
  stk_cout << _T("nbFreeParameter = ") << stat2model.nbFreeParameter() << _T("\n");
  stk_cout << _T("likelihood = ") << stat2model.likelihood() << _T("\n");
  stk_cout << _T("lnLikelihood = ") << stat2model.lnLikelihood() << _T("\n");
  stk_cout << _T("mean =\n") << stat2model.mean() << _T("\n");
  stk_cout << _T("covariance =\n") << stat2model.covariance() << _T("\n");
  stk_cout << _T("\n\n");

  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Successful completion of testing for Normal Law   +\n");
  stk_cout << _T("+ No errors detected.                               +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("\n\n");

  return 0;
}

