/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: June 1, 2016
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_Function.h"
#include "mixt_FunctionalClass.h"

namespace mixt {

void Function::setSize(Index nT,
                       Index nSub) {
  nTime_ = nT;
  nSub_ = nSub;
  t_.resize(nT);
  x_.resize(nT);
  w_.resize(nSub);
}

void Function::setVal(const Vector<Real>& t,
                      const Vector<Real>& x,
                      const Vector<std::set<Index> >& w) {
  nTime_ = t.size();
  nSub_ = w.size();
  t_ = t;
  x_ = x;
  w_ = w;
}

void Function::setValTime(Index i,
                          Real t,
                          Real x) {
  t_(i) = t;
  x_(i) = x;
}

void Function::computeVandermonde(Index nCoeff) {
  vandermondeMatrix(t_, nCoeff, vandermonde_);
}

void Function::computeJointLogProba(const Matrix<Real>& alpha,
                                    const Matrix<Real>& beta,
                                    const Vector<Real>& sd,
                                    Matrix<Real>& jointLogProba) const {
  jointLogProba.resize(nTime_, nSub_);
  NormalStatistic normal;
  Vector<Real> currKappa(nSub_);
  for (Index i = 0; i < nTime_; ++i) {
    kappaMatrix(t_(i), alpha, currKappa);

    for (Index s = 0; s < nSub_; ++s) {
      Real currExpectation = vandermonde_.row(i).dot(beta.row(s)); // since the completed probability is computed, only the current subregression is taken into account in the computation
      Real logAPosteriori = normal.lpdf(x_(i), currExpectation, sd(s));

      Real logAPriori = std::log(currKappa(s));

      jointLogProba(i, s) = logAPriori + logAPosteriori;
    }
  }
}

Real Function::lnCompletedProbability(const Matrix<Real>& alpha,
                                      const Matrix<Real>& beta,
                                      const Vector<Real>& sd) const {
  Real logProba = 0.;

  Matrix<Real> jointLogProba;
  computeJointLogProba(alpha, beta, sd, jointLogProba);
  for (Index s = 0; s < nSub_; ++s) {
    for (std::set<Index>::const_iterator it  = w_(s).begin(),
                                         itE = w_(s).end();
         it != itE;
         ++it) {
      logProba += jointLogProba(*it, s); // only the completed value of w is taken into account
    }
  }

  return logProba;
}

Real Function::lnObservedProbability(const Matrix<Real>& alpha,
                                     const Matrix<Real>& beta,
                                     const Vector<Real>& sd) const {
  Real logProba = 0.;

  Matrix<Real> jointLogProba;
  computeJointLogProba(alpha, beta, sd, jointLogProba);
  for (Index i = 0; i < nTime_; ++i) {
    Vector<Real> dummy;
    logProba += dummy.logToMulti(jointLogProba.row(i)); // every possible value of w is taken into account, to marginalize over w
  }

  return logProba;
}

void Function::sampleWNoCheck(const Matrix<Real>& alpha,
                              const Matrix<Real>& beta,
                              const Vector<Real>& sd) {
  Matrix<Real> jointLogProba;
  computeJointLogProba(alpha, beta, sd, jointLogProba);

  for (Index s = 0; s < nSub_; ++s) {
    w_(s).clear();
  }

  Vector<Real> currProba;
  for (Index i = 0; i < nTime_; ++i) {
    currProba.logToMulti(jointLogProba.row(i));
    w_(multi_.sample(currProba)).insert(i);
  }
}

void Function::sampleWCheck(const Matrix<Real>& alpha,
                            const Matrix<Real>& beta,
                            const Vector<Real>& sd,
                            const FunctionalClass& fc) {
  Matrix<Real> jointLogProba;
  computeJointLogProba(alpha, beta, sd, jointLogProba);

  Vector<Index> w0(nTime_);
  for (Index s = 0; s < nSub_; ++s) {
    for (std::set<Index>::const_iterator it = w_(s).begin(), itE = w_(s).end(); it != itE; ++it) { // building the list of labels
      w0(*it) = s;
    }
  }

  Vector<Real> currProba;
  Vector<Real> validValue(nSub_);
  Vector<Real> sampleProba(nSub_);
  for (Index i = 0; i < nTime_; ++i) {
    currProba.logToMulti(jointLogProba.row(i));

    for (Index s = 0; s < nSub_; ++s) {
      w_(w0(i)).erase(i);
      w_(s).insert(i);
      w0(i) = s;

      validValue(s) = fc.checkSampleCondition();
    }
    w_(w0(i)).erase(i);
    sampleProba = currProba % validValue; // coefficient-wise multiplication

    if (sampleProba == 0.) { // might happen in some extreme cases
      sampleProba = validValue / validValue.sum();
    }
    else {
      sampleProba = sampleProba / sampleProba.sum();
    }

    w_(multi_.sample(sampleProba)).insert(i);
  }
}

void Function::removeMissingUniformSampling() {
  for (Index s = 0; s < nSub_; ++s) { // clearing is necessary, as removeMissing will be called at several points during the run
    w_(s).clear();
  }

  for (Index i = 0; i < nTime_; ++i) {
    w_(multi_.sampleInt(0, nSub_ - 1)).insert(i); // w follows a uniform discrete law
  }
}

void Function::removeMissingQuantile(const Vector<Real>& quantiles) {
  Index nQuantile = quantiles.size();
  for (Index s = 0; s < nSub_; ++s) { // clearing is necessary, as removeMissing will be called at several points during the run
    w_(s).clear();
  }

  for (Index i = 0; i < nTime_; ++i) {
    if (t_(i) < quantiles(1)) {
      w_(0).insert(i);
    }
    else if (quantiles(nQuantile - 2) < t_(i)) {
      w_(nQuantile - 2).insert(i);
    }
    else {
      for (Index q = 1; q < nQuantile - 2; ++q) {
        if (quantiles(q) < t_(i)) {
          w_(q).insert(i);
          break;
        }
      }
    }
  }

#ifdef MC_DEBUGNEW
  Index nW = 0; // total number of assigned w, to check that all times have been assigned
  for (Index s = 0; s < nSub_; ++s) {
    nW += w_(s).size();
  }
  std::cout << "nW: " << nW << ", nTime_: " << nTime_ << std::endl;
#endif
}

void Function::removeMissingQuantileMixing(const Vector<Real>& quantiles) {
  for (Index s = 0; s < nSub_; ++s) { // clearing is necessary, as removeMissing will be called at several points during the run
    w_(s).clear();
  }

  Vector<Real> midPoints(nSub_);
  Vector<Real> proba(nSub_);

  for (Index s = 0; s < nSub_; ++s) {
    midPoints(s) = (quantiles(s + 1) + quantiles(s)) / 2.;
  }
//  std::cout << "midPoints: " << itString(midPoints) << std::endl;

  for (Index i = 0; i < nTime_; ++i) {
    Real currT = t_(i);
    proba = 0.;

    if (currT < midPoints(0)) {
      proba(0) = 1.;
    }
    else if (midPoints(nSub_ - 1) < currT) {
      proba(nSub_ - 1) = 1.;
    }
    else {
      for (Index s = 0; s < nSub_ - 1; ++s) {
        Real disLeft  = currT - midPoints(s);
        Real disRight = midPoints(s + 1) - currT;
        proba(s    ) = disRight / (disLeft + disRight);
        proba(s + 1) = disLeft  / (disLeft + disRight);
      }
    }

//    std::cout << "currT: " << currT << ", proba: " << itString(proba) << std::endl;

    Index currW = multi_.sample(proba);
    w_(currW).insert(i);
  }

#ifdef MC_DEBUGNEW
  std::cout << "Function::removeMissingQuantileMixing" << std::endl;
  Index nW = 0; // total number of assigned w, to check that all times have been assigned
  for (Index s = 0; s < nSub_; ++s) {
    nW += w_(s).size();
    std::cout << itString(w_(s)) << std::endl;
  }
  std::cout << "nW: " << nW << ", nTime_: " << nTime_ << std::endl;
#endif
}

double Function::costAndGrad(unsigned nParam,
                             const double* alpha,
                             double* grad) const {
  double cost;
  Matrix<Real> logValue;
  Vector<Real> logSumExpValue;

  timeValue(t_,
            nParam,
            alpha,
            logValue,
            logSumExpValue);

  costFunction(t_,
               logValue,
               logSumExpValue,
               w_,
               cost);

  if (grad != NULL) {
    gradCostFunction(t_,
                     logValue,
                     logSumExpValue,
                     w_,
                     grad);
  }

  return cost;
}

void Function::printProp() const {
  Vector<Real> prop(nSub_, 0.);

  for (Index s = 0; s < nSub_; ++s) {
    prop(s) = w_(s).size();
  }

  prop = prop / prop.sum();

  std::cout << "Prop of w: " << itString(prop) << std::endl;
}

void Function::quantile(Vector<Real>& quantile) {
  Index nQuantile = nSub_ + 1;
  quantile.resize(nQuantile);

  Vector<Real> sortedT = t_; // copy of t to be sorted, as sorted time are not required by the model

  sortedT.sort();

  Real quantileSize = 1. / nSub_;

  quantile(0) = sortedT(0);
  quantile(nQuantile - 1) = sortedT(nTime_ - 1);

  for (Index q = 1; q < nQuantile - 1; ++q) {
    quantile(q) = sortedT(q * quantileSize * (nTime_ - 1));
  }
}

} // namespace mixt
