#include <iostream>
#include "IterationsStrategy.h"
#include "framework/util/util.h"

IterationsStrategy::IterationsStrategy(int iterations,int burnin, int nbtry):iterations_(iterations),
                                                                             burnin_(burnin),
                                                                             nbtry_(nbtry)
{}

IterationsStrategy::~IterationsStrategy() {
}

void IterationsStrategy::run(IAlgo* p_algo_,Model*& p_model_)
{
  double likelihood = -RealMax;
  Model * currentmodel = p_model_->clone();
  p_model_->setData();
  currentmodel->setData();
  for (int i = 0; i < nbtry_; ++i)
  {
    std::cout<<"Try: "<<i<<"\n";
    currentmodel->initializeModel();
    for (int itr = 0; itr < iterations_; ++itr) {
      p_algo_->run(currentmodel);
      if (itr>=burnin_) {
        currentmodel->storeIntermediateResults(itr-burnin_);
      }
    }
    currentmodel->finalizeModel();
    if(currentmodel->logLikelihood()>likelihood){
      *p_model_ = *currentmodel;
      likelihood = currentmodel->logLikelihood();
    }
  }
  delete currentmodel;
}
