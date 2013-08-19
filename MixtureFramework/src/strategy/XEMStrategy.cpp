#include <limits>
#include <cmath>
#include "XEMStrategy.h"


XEMStrategy::XEMStrategy(XEMstratParam& param):param_(param)
{}

XEMStrategy::~XEMStrategy()
{}

void XEMStrategy::run(IModel*& p_model,IAlgo*& p_algo,IInit*& p_init){
  IModel* currentmodelshort = p_model->create();
  IModel* currentmodellong = p_model->create();

  double bestlikelihood = std::numeric_limits<double>::min();

  for (int nblongrun = 0; nblongrun < param_.nbLongruns_; ++nblongrun) {
    double currentlikelihoodlong = std::numeric_limits<double>::min();

    for (int nbshortrun = 0; nbshortrun < param_.nbShortruns_; ++nbshortrun) {
      double currentlikelihoodshort = std::numeric_limits<double>::min();
      double currentbestlikelihoodshort = std::numeric_limits<double>::min();
      p_init->run(currentmodelshort);
      for (int itr = 0; itr < param_.nbShortIterations_; ++itr) {
        p_algo->run(currentmodelshort);
        currentlikelihoodshort = currentmodelshort->lnLikelihood();
        if(std::abs(currentbestlikelihoodshort-currentlikelihoodshort)<param_.epsilonShort_) break;
        currentbestlikelihoodshort = currentlikelihoodshort;
      }

      if(currentlikelihoodlong<currentlikelihoodshort){
        delete currentmodellong;
        currentmodellong = currentmodelshort->clone();
        currentlikelihoodlong = currentlikelihoodlong;
      }
    }

    double currentbestlikelihoodlong = currentlikelihoodlong;
    for (int itr = 0; itr < param_.nbLongIterations_; ++itr) {
      p_algo->run(currentmodellong);
      currentlikelihoodlong = currentmodellong->lnLikelihood();
      if(std::abs(currentbestlikelihoodlong-currentlikelihoodlong)<param_.epsilonLong_) break;
      currentbestlikelihoodlong = currentlikelihoodlong;
    }

    if(bestlikelihood<currentlikelihoodlong){
      delete p_model;
      p_model = currentmodellong->clone();
      bestlikelihood = currentlikelihoodlong;
    }
  }

  delete currentmodellong;
  delete currentmodelshort;
}
