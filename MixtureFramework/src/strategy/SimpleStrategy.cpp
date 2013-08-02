#include <limits>
#include <cmath>
#include "SimpleStrategy.h"

SimpleStrategy::SimpleStrategy(SimpleStratParam& param):param_(param)
{}

SimpleStrategy::~SimpleStrategy()
{}


void SimpleStrategy::run(IModel*& p_model,IAlgo*& p_algo,IInit*& p_init)
{
  IModel* currentmodel = p_model->create();
  double bestlikelihood = std::numeric_limits<double>::min();
  for (int nbtry = 0; nbtry < param_.nbTry_; ++nbtry)
  {
    double currentlikelihood = std::numeric_limits<double>::min();
    double currentbestlikelihood = std::numeric_limits<double>::min();
    p_init->run(currentmodel);
    for (int itr = 0; itr < param_.nbIterations_; ++itr)
    {
      p_algo->run(currentmodel);
      currentlikelihood = currentmodel->lnLikelihood();
      std::cout << "In SimpleStrategy::run. Current likelihood =" << currentlikelihood << "\n";
      if(std::abs(currentbestlikelihood-currentlikelihood)<param_.epsilon_) break;

      currentbestlikelihood = currentlikelihood;
    }
    if(bestlikelihood<currentlikelihood)
    {
     *p_model = *currentmodel;
     bestlikelihood = currentlikelihood;
    }
  }

  delete currentmodel;
}
