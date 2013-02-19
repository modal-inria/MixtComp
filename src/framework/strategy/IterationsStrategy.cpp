#include "IterationsStrategy.h"

IterationsStrategy::IterationsStrategy(int iterations, int nbtry):iterations_(iterations),
                                                                  nbtry_(nbtry)

IterationsStrategy::~IterationsStrategy() {
  // TODO Auto-generated destructor stub
}

void IterationsStrategy::run() {
  // TODO storing the best model
  for (int i = 0; i < nbtry_; ++i) {
    p_model_->initializeModel();
    for (int i = 0; i < iterations_; ++i) {
      p_algo_->run();
    }
    p_model_->finalizeModel();
  }
}
