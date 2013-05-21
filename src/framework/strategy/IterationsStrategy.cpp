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
  for (int i = 0; i < nbtry_; ++i)
  {
    std::cout<<"Try: "<<i<<"\n";
    Model * currentmodel = p_model_->clone();
    currentmodel->setData();
    currentmodel->initializeModel();
    for (int itr = 0; itr < iterations_; ++itr) {
      p_algo_->run(currentmodel);
      if (itr>=burnin_) {
        currentmodel->storeIntermediateResults(itr-burnin_);
      }
    }
    currentmodel->finalizeModel();
    if(currentmodel->logLikelihood()>likelihood){
      if(p_model_!=NULL) delete p_model_;
      p_model_ = currentmodel->clone();
      likelihood = currentmodel->logLikelihood();
    }
    delete currentmodel;
  }
}
