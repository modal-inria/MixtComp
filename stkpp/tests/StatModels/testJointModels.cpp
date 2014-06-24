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

    Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
*/

/*
 * Project:  stkpp::tests
 * created on: 8 août 2011
 * Purpose:  test the Normal and MultiNormal classes.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file testJointModels.cpp
 *  @brief In this file we test the joint Statistical models.
 **/

#include "STatistiK.h"
#include "STatistiK/include/STK_Law_Util.h"

#include "StatModels.h"

using namespace STK;

template<class Array, class Model, class Law >
void testModel(Array& data, Law const& law)
{
  typedef typename Array::Row RowVector;
  // simulate data set
  for (Integer i= data.firstIdxRows(); i <= data.lastIdxRows(); ++i)
  {
    RowVector row(data.row(i), true); // reference on the ith row
    law.rand(row);
  }
  // create model and initialize parameters
  Model model(data);
  model.createParameters();
  // estimate model
  if (!model.run())
  { stk_cout << _T("An error occurred in testModel.\nWhat: ") << model.error() << _T("\n");}
  stk_cout << _T("Estimated parameters=\n");
  model.writeParameters(stk_cout);
  stk_cout << "lnLikelihood =" << model.lnLikelihood() << _T("\n");
  stk_cout << "nbFreeParameter =" << model.nbFreeParameter() << _T("\n\n");
}

template<class Array, class Model, class Law >
void testWeightedModel(Array const& data, Law const& law, CVectorX const& weights)
{
  // create model and initialize parameters
  Model model(data);
  model.createParameters();
  // estimate model
  if (!model.run(weights))
  { stk_cout << _T("An error occurred in testModel.\nWhat: ") << model.error() << _T("\n");}
  stk_cout << _T("Estimated parameters=\n");
  model.writeParameters(stk_cout);
  stk_cout << "lnLikelihood =" << model.lnLikelihood() << _T("\n");
  stk_cout << "nbFreeParameter =" << model.nbFreeParameter() << _T("\n\n");
}

template<class Array, class Model>
void testCloneModel(Array const& data, Model& model)
{
  // create model and initialize parameters
  model.setData(data);
  model.createParameters();
  // estimate model
  if (!model.run())
  { stk_cout << _T("An error occurred in testCloneModel.\nWhat: ") << model.error() << _T("\n");}
  Model* p_model = model.clone();
  stk_cout << _T("Estimated parameters=\n");
  p_model->writeParameters(stk_cout);
  delete p_model;
  stk_cout << "lnLikelihood =" << model.lnLikelihood() << _T("\n");
  stk_cout << "nbFreeParameter =" << model.nbFreeParameter() << _T("\n\n");
}

int main(int argc, char *argv[])
{
  int N = (argc < 2) ? 200 : int(atoi(argv[1]));
  // weigths are all equals
  CVectorX w(N, 1./Real(N));

  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Test JointBernoulliModel                          +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("\n\n");

  // create Bernoulli joint law
  Law::JointBernoulli<Array2DPoint<Binary> > bLaw(20);
  CPointX prob(20);
  for (Integer j= bLaw.JointLaw().firstIdx(); j <= bLaw.JointLaw().lastIdx(); ++j)
  {
    prob[j] = Law::generator.randUnif();
    bLaw.setProb(j, prob[j]);
  }
  // create data set
  Array2D<Binary> bData(N, 20);
  stk_cout << _T("True probabilities=\n") << prob;
  testModel< Array2D<Binary>, JointBernoulliModel<Array2D<Binary> > >(bData, bLaw);
  stk_cout << _T("Test with weights\n");
  testWeightedModel< Array2D<Binary>, JointBernoulliModel<Array2D<Binary> > >(bData, bLaw, w);

  // create model
  JointBernoulliModel<Array2D<Binary> > bModel;
  stk_cout << _T("True probabilities=\n") << prob;
  testCloneModel(bData, bModel);

  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Successful completion of testing for              +\n");
  stk_cout << _T("+ JointBernoulliModel.                              +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("\n\n");

  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Test JointGaussianModel                           +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("\n\n");

  // run model
  Law::JointNormal<Array2DPoint<Real> > nLaw(20);
  CPointX mu(20);
  CPointX sigma(20);
  for (Integer j= nLaw.JointLaw().firstIdx(); j <= nLaw.JointLaw().lastIdx(); ++j)
  {
    mu[j] = Law::generator.randGauss();
    sigma[j] = Law::generator.randExp();
    nLaw.setMu(j, mu[j]);
    nLaw.setSigma(j, sigma[j]);
  }
  // create data set
  Array2D<Real> nData(N, 20);
  // test Joint Gaussian model
  stk_cout << _T("True mu and sigma=\n") << mu << sigma;
  testModel< Array2D<Real>, JointGaussianModel<Array2D<Real> > >(nData, nLaw);
  stk_cout << _T("\nTest with weights\n");
  testWeightedModel< Array2D<Real>, JointGaussianModel<Array2D<Real> > >(nData, nLaw, w);

  // create model
  JointGaussianModel<Array2D<Real> > nModel;
  stk_cout << _T("\nTrue mu and sigma=\n") << mu << sigma;
  testCloneModel(nData, nModel);

  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Successful completion of testing for              +\n");
  stk_cout << _T("+ JointGaussian Model.                              +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("\n\n");

  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Test JointGammaModel                           +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("\n\n");

  // run model
  // create Bernoulli joint law
  Law::JointGamma<Array2DPoint<Real> > gLaw(20);
  CPointX alpha(20), beta(20);
  for (Integer j= gLaw.JointLaw().firstIdx(); j <= gLaw.JointLaw().lastIdx(); ++j)
  {
    alpha[j] = Law::generator.randExp()*5;
    beta[j] = Law::generator.randExp();
    gLaw.setShape(j, alpha[j]);
    gLaw.setScale(j, beta[j]);
  }
  // create data set
  Array2D<Real> gData(N, 20);
  // create model
  stk_cout << _T("True alpha and beta=\n") << alpha << beta;
  testModel< Array2D<Real>, JointGammaModel<Array2D<Real>, CVectorX >, Law::JointGamma<Array2DPoint<Real> > >(gData, gLaw);
  stk_cout << _T("Test with weights\n");
  testWeightedModel< Array2D<Real>, JointGammaModel<Array2D<Real>, CVectorX >, Law::JointGamma<Array2DPoint<Real> > >(gData, gLaw, w);

  stk_cout << _T("True alpha and beta=\n") << alpha << beta;
  JointGammaModel<Array2D<Real>, CVectorX > gModel;
  testCloneModel(gData, gModel);

  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Successful completion of testing for              +\n");
  stk_cout << _T("+ JointGaussian Model.                              +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("\n\n");
  return 0;
}

