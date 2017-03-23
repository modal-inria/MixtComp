/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: June 7, 2016
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_FUNCTIONALCLASS
#define MIXT_FUNCTIONALCLASS

#include "LinAlg/mixt_LinAlg.h"
#include "Param/mixt_ConfIntParamStat.h"
#include "mixt_Function.h"
#include "mixt_FunctionalComputation.h"

namespace mixt {

class FunctionalClass {
  public:
    FunctionalClass(Vector<Function>& data,
                    const std::set<Index>& setInd,
                    Real confidenceLevel);

    void setSize(Index nSub,
                 Index nCoeff);

    template<typename MatrixType,
             typename VectorType>
    void setParam(const MatrixType& alpha,
                  const MatrixType& beta,
                  const VectorType& sd) {
      alpha_ = alpha;
      beta_ = beta;
      sd_ = sd;
    }

    void mStep();

    void mStepAlpha();

    void mStepBetaSd();

    void sampleParam();

    void setExpectationParam();

    void setParamStorage();

    void sampleParam(Index iteration,
                     Index iterationMax);

    double costAndGrad(Index nFreeParam,
                       const double* alpha,
                       double* grad);

    const Matrix<Real>& alpha() const {return alpha_;}
    const Matrix<Real>& beta()  const {return beta_ ;}
    const Vector<Real>& sd()    const {return sd_   ;}

    const ConfIntParamStat<Matrix<Real> >& alphaParamStat() const {return alphaParamStat_;}
    const ConfIntParamStat<Matrix<Real> >& betaParamStat()  const {return betaParamStat_ ;}
    const ConfIntParamStat<Vector<Real> >& sdParamStat()    const {return sdParamStat_   ;}

    void samplingStepNoCheck(Index i);

    void samplingStepCheck(Index i);

    Index checkSampleCondition(std::string* warnLog = NULL) const;
    bool checkNbDifferentValue() const;
    bool checkNonNullSigma() const;
  private:
    Index nSub_;

    Index nCoeff_;
    /** Data, not const because sampling has to occur at some point */
    Vector<Function>& data_;

    /** List of individuals among the data that belong to the class corresponding to this RankClass. */
    const std::set<Index>& setInd_;

    /** Parameter alpha for current class. Not set as const since mStep will modify it. It is not stored in a matrix, because nlopt is used,
     * and serializing everything in a vector is the standard modus operandi. */
    Matrix<Real> alpha_;

    /** Rows are sub-regression, columns are coefficients. */
    Matrix<Real> beta_;

    /** One coefficient per sub-regression */
    Vector<Real> sd_;

    /** Contrarily to what is done in the Rank model for example, here the parameter estimation is done
     * directly inside class. */
    ConfIntParamStat<Matrix<Real> > alphaParamStat_;
    ConfIntParamStat<Matrix<Real> > betaParamStat_;
    ConfIntParamStat<Vector<Real> > sdParamStat_;
};

} // namespace mixt

#endif // MIXT_FUNCTIONALCLASS
