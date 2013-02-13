#include "MixtCompFacade.h"

MixtCompFacade::MixtCompFacade()
{
  // TODO Auto-generated constructor stub

}

MixtCompFacade::~MixtCompFacade()
{
  if(p_model_) {
    delete p_model_;
    p_model_ = NULL;
  }

  if(p_algo_){
    delete p_algo_;
    p_algo_ = NULL;
  }

  if(p_strategy_){
    delete p_strategy_;
    p_strategy_ = NULL;
  }

  if(p_developer_){
    delete p_developer_;
    p_developer_ = NULL;
  }

  for (int i = 0; i < v_developer_.size(); ++i) {
    if(v_developer_[i]) {
      delete v_developer_[i];
      v_developer_[i] = NULL;
    }
  }
}

void MixtCompFacade::instantiateFramework(){
  //TODO creation of v_developer_
  for(MixtureLaw law: info.mixturelawlist_){
    switch (law) {
      case multinomial_:

        break;
      default:
        break;
    }
  }

  p_developer_ = new CompositeDeveloper(v_developer_);
  //create model
  p_model_ = new Model();
  //create algorithm
  switch (Algo) {
    case semgibbs_:
      p_algo_ = new SEMGibbs();
      break;
    default:
      p_algo_ = new SEMGibbs();
      break;
  }
  //create strategy
  switch (strategy) {
    case iterations_:
      p_strategy_ = new IterationsStrategy();
      break;
    default:
      p_strategy_ = new IterationsStrategy();
      break;
  }

  //set various links
  p_algo_->setModel(p_model_);
  p_strategy_->setAlgo(p_algo_);
}
