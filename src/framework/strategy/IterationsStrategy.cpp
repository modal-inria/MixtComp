#include "IterationsStrategy.h"

IterationsStrategy::IterationsStrategy(int iterations)
{
  iterations_ = iterations;
}

IterationsStrategy::~IterationsStrategy()
{
  // TODO Auto-generated destructor stub
}

void IterationsStrategy::run(){
  for (int i = 0; i < iterations_; ++i) {
    p_algo_->run();
  }
}
