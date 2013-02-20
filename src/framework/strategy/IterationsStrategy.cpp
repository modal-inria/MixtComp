#include "IterationsStrategy.h"
#include "framework/util/util.h"

IterationsStrategy::IterationsStrategy(int iterations, int nbtry):iterations_(iterations),
                                                                  nbtry_(nbtry){}

IterationsStrategy::~IterationsStrategy() {
  // TODO Auto-generated destructor stub
}

void IterationsStrategy::run() {
  double likelihood = -RealMax;
  Model * currentmodel = p_model_->clone();
  for (int i = 0; i < nbtry_; ++i) {
    currentmodel->initializeModel();
    for (int i = 0; i < iterations_; ++i) {
      p_algo_->run(currentmodel);
    }
    currentmodel->finalizeModel();
    if(currentmodel->logLikelihood()>likelihood){
      p_model_ = currentmodel;
      likelihood = currentmodel->logLikelihood();
    }
  }

  //release memory
  delete currentmodel;
}
