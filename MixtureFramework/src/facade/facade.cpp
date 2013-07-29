#include "facade.h"

facade::facade()
{
  // TODO Auto-generated constructor stub

}

facade::facade(IModel*& model,IAlgo*& algo,IInit*& init,IStrategy*& strat){
  p_Algo_ = algo;
  p_Model_ = model;
  p_Init_ = init;
  p_Strategy_ = strat;
}

facade::~facade()
{
  // TODO Auto-generated destructor stub
}
