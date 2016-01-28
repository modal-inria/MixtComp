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
 *  Created on: June 5, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_CONFINTPARAMSTAT_H
#define MIXT_CONFINTPARAMSTAT_H

#include "../LinAlg/mixt_LinAlg.h"

namespace mixt
{

/**
 * Computation of confidence interval on parameters. Templated for int or Real cases.
 * */
template<typename Type>
class ConfIntParamStat
{
  public:
    ConfIntParamStat(Vector<Type>& param,
                     Real confidenceLevel) :
      nbIter_(0),
      nbParam_(0),
      param_(param),
      confidenceLevel_(confidenceLevel)
    {};

    void sampleParam(int iteration,
                     int iterationMax)
    {
      if (iteration == 0)
      {
        nbParam_ = param_.rows();
        logStorage_.resize(nbParam_, iterationMax + 1); // resize internal storage
        statStorage_.resize(nbParam_, 3); // resize export storage

        sample(0); // first sampling, on each parameter
      }
      else if (iteration == iterationMax)
      {
        sample(iterationMax); // last sampling

        for (int p = 0; p < nbParam_; ++p)
        {
    #ifdef MC_DEBUG
        std::cout << "SimpleParamStat::sampleParam: " << std::endl;
        std::cout << "\tp: " << p << std::endl;
    #endif

          RowVector<Type> currRow = logStorage_.row(p);
          currRow.sort();

          Real alpha = (1. - confidenceLevel_) / 2.;
          int realIndLow =        alpha  * iterationMax;
          int realIndHigh = (1. - alpha) * iterationMax;

          statStorage_(p, 0) = currRow(iterationMax / 2);
          statStorage_(p, 1) = currRow(realIndLow      );
          statStorage_(p, 2) = currRow(realIndHigh + 1 );
        }
      }
      else
      {
        sample(iteration); // standard sampling
      }
    }

    /** Set the storage of parameters, for example in the prediction case where there is no M-Step, and no
     * statistics on the estimated parameters. */
    void setParamStorage()
    {
      statStorage_.resize(param_.rows(),
                          1); // no quantiles have to be computed for imported parameters, hence the single column
      statStorage_.col(0) = param_;
    }

    /** fill the parameters with estimators of the expectation, to be used in the Gibbs sampling */
    void setExpectationParam() {
      for (int i = 0; i < nbParam_; ++i) {
        param_(i) = statStorage_(i, 0); // imputation by the mean
      }
    }

    /** Perform renormalization on statStorage. Useful for categorical modes where umputed parameters must
     * sum to 1 */
    void normalizeParam(int nbClass, int nbModality) {
      for (int j = 0; j < statStorage_.cols(); ++j) {
        for (int k = 0; k < nbClass; ++k) {
          Type sumClass = 0;
          for (int p = 0; p < nbModality; ++p) {
            sumClass += statStorage_(k * nbModality + p, j);
          }
          for (int p = 0; p < nbModality; ++p) {
            statStorage_(k * nbModality + p, j) /= sumClass;
          }
        }
      }
    }

    const Matrix<Type>& getStatStorage() const {return statStorage_;};
    const Matrix<Type>& getLogStorage() const {return logStorage_;};
  private:
    void sample(int iteration)
    {
      for (int p = 0; p < nbParam_; ++p)
      {
        logStorage_(p, iteration) = param_(p);;
      }
    }

    // number of iterations used to compute the statistics
    int nbIter_;

    // number of parameters
    int nbParam_;

    // Reference to param array
    Vector<Type>& param_;

    /** Array to export the statistics at the last iteration
     * first dimension: index of the parameter
     * second dimension: median, left quantile, right quantile */
    Matrix<Type> statStorage_;

    /** Storage for iterations results,
     * first dimension: index of the parameter
     * second dimension: iteration of the stored value */
    Matrix<Type> logStorage_;

    /** Confidence level */
    Real confidenceLevel_;


};

} // namespace mixt

#endif // MIXT_INTCONFPARAMSTAT_H
