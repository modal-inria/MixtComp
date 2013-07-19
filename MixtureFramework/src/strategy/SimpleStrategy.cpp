#include <limits>
#include <cmath>
#include "SimpleStrategy.h"

SimpleStrategy::SimpleStrategy()
{
  // TODO Auto-generated constructor stub

}

SimpleStrategy::~SimpleStrategy()
{
  // TODO Auto-generated destructor stub
}


void SimpleStrategy::run(IModel*& p_model,IAlgo*& p_algo,IInit*& p_init){
  IModel* currentmodel = p_model->create();
  double bestlikelihood = std::numeric_limits<double>::min();
  for (int nbtry = 0; nbtry < param_.nbTry_; ++nbtry) {
    double currentlikelihood = std::numeric_limits<double>::min();
    double currentbestlikelihood = std::numeric_limits<double>::min();
    p_init->run(currentmodel);
    for (int itr = 0; itr < param_.nbIterations_; ++itr) {
      p_algo->run(currentmodel);
      currentlikelihood = currentmodel->lnLikelihood();
      if(std::abs(currentbestlikelihood-currentlikelihood)<param_.epsilon_) break;

      currentbestlikelihood = currentlikelihood;
    }
    if(bestlikelihood<currentlikelihood){
     *p_model = *currentmodel;
     bestlikelihood = currentlikelihood;
    }
  }

  delete currentmodel;
}
