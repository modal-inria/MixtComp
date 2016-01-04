/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2015

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
 *  Created on: July 9, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "../UTest.h"

using namespace mixt;

/**
 * Test double inversion of a matrix
 */
TEST(Statistics, MultinomialSeed)
{
  int nbMulti = 100;
  int nbModalities = 4;
  RowVector<Real> prop(nbModalities);
  prop = 1. / Real(nbModalities);

  Vector<MultinomialStatistic> multi(nbMulti);
  Vector<Real> sample(nbMulti);

  for (int i = 0; i < nbMulti; ++i)
  {
    sample(i) = multi(i).sample(prop);
  }

  Real mean = sample.mean();
  Real var = (sample - mean).transpose() * ((sample - mean)); // 1 / N factor omitted

#ifdef MC_DEBUG
  std::cout << "sample" << std::endl;
  std::cout << sample << std::endl;
  std::cout << "var: " << var << std::endl;
#endif

  ASSERT_GT(var, epsilon);
}

/**
 * Test sampling Poisson conditional to non-zero value
 * */
TEST(Statistics, NonZeroPoissonSampling)
{
  int lambda = 5;
  int nbInd = 1000000;
  int nbBin = 100;

  PoissonStatistic poisson;
  RowVector<Real> sampleFreq(nbBin, 0.);
  RowVector<Real> compFreq(nbBin, 0.);

  for  (int i = 0; i < nbInd; ++i)
  {
    sampleFreq(poisson.nonZeroSample(lambda)) += 1.;
  }

  for (int p = 1; p < nbBin; ++p) // note that the modality 0 is ignored
  {
    compFreq(p) = poisson.pdf(p, lambda);
  }

  sampleFreq = sampleFreq / sampleFreq.sum();
  compFreq   = compFreq   / compFreq  .sum();
  RowVector<Real> error = sampleFreq - compFreq;
  Real maxAbsError = error.abs().maxCoeff();

#ifdef MC_DEBUG
  std::cout << "sampleFreq" << std::endl;
  std::cout << sampleFreq << std::endl;
  std::cout << "compFreq" << std::endl;
  std::cout << compFreq << std::endl;
  std::cout << "error" << std::endl;
  std::cout << error << std::endl;
  std::cout << "maxAbsError: " << maxAbsError << std::endl;
#endif

  ASSERT_LT(maxAbsError, 1.e-3);
}

/**
 * Test distribution obtained from MultinomialStatistic::shuffle
 * */
TEST(Statistics, Shuffle)
{
  int nbPos = 10;
  int nbSample = 10000;

  Vector<int> ind(nbPos);
  Matrix<int> res(nbSample,
                  nbPos,
                  -1);
  Matrix<Real> proba(nbPos,
                     nbPos,
                     0.);
  RowVector<Real> h(nbPos);

  MultinomialStatistic multi;

  for (int i = 0; i < nbSample; ++i)
  {
    for (int p = 0; p < nbPos; ++p)
    {
      ind(p) = p;
    }

    multi.shuffle(ind);

    for (int p = 0; p < nbPos; ++p)
    {
      proba(ind(p), p) += 1.;
    }

    res.row(i) = ind;
  }

  proba /= nbSample;

  for (int p = 0; p < nbPos; ++p)
  {
    h(p) = multi.entropy(proba.col(p));
  }

  RowVector<Real> expectedEntropy(nbPos, std::log(1. / nbPos));

  Real maxErr = (h - expectedEntropy).abs().maxCoeff();

#ifdef MC_DEBUG
  std::cout << "proba" << std::endl;
  std::cout << proba << std::endl;
  std::cout << "h" << std::endl;
  std::cout << h << std::endl;
  std::cout << "expectedEntropy" << std::endl;
  std::cout << expectedEntropy << std::endl;
#endif

  ASSERT_LT(maxErr, 0.01);
}
