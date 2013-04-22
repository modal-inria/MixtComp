#include "IterationsStrategy.h"
#include "framework/util/util.h"

IterationsStrategy::IterationsStrategy(int iterations,int burnin, int nbtry):iterations_(iterations),
                                                                             burnin_(burnin),
                                                                             nbtry_(nbtry)
{}

IterationsStrategy::~IterationsStrategy() {
}

void IterationsStrategy::run(IAlgo* p_algo_,Model* p_model_) {
  double likelihood = -RealMax;
  Model * currentmodel = new Model(p_model_);
  for (int i = 0; i < nbtry_; ++i) {
    currentmodel->initializeModel();
    for (int i = 0; i < iterations_; ++i) {
      p_algo_->run(currentmodel);
      if (i>burnin_) {
        p_model_->storeIntermediateResults(i-burnin_);
      }
    }
    currentmodel->finalizeModel();
    if(currentmodel->logLikelihood()>likelihood){
      *p_model_ = *currentmodel;
      likelihood = currentmodel->logLikelihood();
    }
  }

  //release memory
  delete currentmodel;
}
