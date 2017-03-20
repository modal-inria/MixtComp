/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: June 5, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_CONFINTPARAMSTAT_H
#define MIXT_CONFINTPARAMSTAT_H

#include "boost/regex.hpp"

#include "LinAlg/mixt_LinAlg.h"
#include "IO/mixt_IO.h"
#include "IO/mixt_SpecialStr.h"

namespace mixt {

/**
 * Computation of confidence interval on parameters. Templated for int or Real cases.
 * */
template<typename ContainerType>
class ConfIntParamStat {
  public:
    typedef typename ContainerType::Type Type;

    ConfIntParamStat(ContainerType& param,
                     Real confidenceLevel) :
      nbIter_(0),
      nRows_(0),
      nCols_(0),
      nCoeff_(0),
      param_(param),
      confidenceLevel_(confidenceLevel) {};

    void sampleParam(Index iteration,
                     Index iterationMax) {
      if (iteration == 0) {
        nRows_ = param_.rows(); // size of nbParam is know at the first sample and from there on will stay fixed
        nCols_ = param_.cols();
        nCoeff_ = nRows_ * nCols_;

        logStorage_ .resize(nCoeff_, iterationMax + 1); // resize internal storage
        statStorage_.resize(nCoeff_, 3               ); // resize export storage

        sample(0); // first sampling, on each parameter
      }
      else if (iteration == iterationMax) {
        sample(iterationMax); // last sampling

        for (Index p = 0; p < nCoeff_; ++p) {
          RowVector<Type> currRow = logStorage_.row(p);
          currRow.sort();

          Real alpha = (1. - confidenceLevel_) / 2.;
          Index realIndLow  =       alpha  * iterationMax;
          Index realIndHigh = (1. - alpha) * iterationMax;

          statStorage_(p, 0) = currRow(iterationMax / 2);
          statStorage_(p, 1) = currRow(realIndLow      );
          statStorage_(p, 2) = currRow(realIndHigh + 1 );
        }
      }
      else {
        sample(iteration); // standard sampling
      }
    }

    /** Set the storage of parameters, for example in the prediction case where there is no M-Step, and no
     * statistics on the estimated parameters. */
    void setParamStorage() {
      nRows_ = param_.rows(); // size of nbParam is know at the first sample and from there on will stay fixed
      nCols_ = param_.cols();
      nCoeff_ = nRows_ * nCols_;

      statStorage_.resize(nCoeff_,
                          1); // no quantiles have to be computed for imported parameters, hence the single column
      for (Index j = 0; j < nCols_; ++j) {
        for (Index i = 0; i < nRows_; ++i) {
          statStorage_(i * nCols_ + j, 0) = param_(i, j);
        }
      }
    }

    /** fill the parameters with estimators of the expectation, to be used in the Gibbs sampling */
    void setExpectationParam() {
      for (Index j = 0; j < nCols_; ++j) {
        for (Index i = 0; i < nRows_; ++i) {
          param_(i, j) = statStorage_(i * nCols_ + j, 0);
        }
      }
    }

    /** Perform renormalization on statStorage. Useful for categorical modes where imputed parameters must
     * sum to 1 */
    void normalizeParam(const std::string& paramStr) {
      Index nModality = 0;

      std::string nModStr = std::string("nModality: *") + strPositiveInteger;
      boost::regex nModRe(nModStr);
      boost::smatch matchesVal;

      if (boost::regex_match(paramStr, matchesVal, nModRe)) { // value is present
        nModality = str2type<Index>(matchesVal[1].str());
      }

      if (nModality > 0) {
        Index nClass = param_.size() / nModality;

        for (Index j = 0; j < statStorage_.cols(); ++j) { // note that the normalization is done column by column...
          for (Index k = 0; k < nClass; ++k) {
            Type sumClass = 0; // ... and class by class
            for (Index p = 0; p < nModality; ++p) {
              sumClass += statStorage_(k * nModality + p, j);
            }
            for (Index p = 0; p < nModality; ++p) {
              statStorage_(k * nModality + p, j) /= sumClass;
            }
          }
        }
      }
    }

    const Matrix<Type>& getStatStorage() const {return statStorage_;}
    const Matrix<Type>& getLogStorage()  const {return logStorage_ ;}

  private:
    void sample(Index iteration) {
      for (Index j = 0; j < nCols_; ++j) {
        for (Index i = 0; i < nRows_; ++i) {
          logStorage_(i * nCols_ + j, iteration) = param_(i, j);
        }
      }
    }

    // number of iterations used to compute the statistics
    Index nbIter_;

    // number of parameters
    Index nRows_;
    Index nCols_;
    Index nCoeff_;

    // Reference to param array
    ContainerType& param_;

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
