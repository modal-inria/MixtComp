#include "framework/DataHandling/DataHandler.h"
#include "MixtCompFacade.h"

MixtCompFacade::MixtCompFacade(FrameworkInfo& info): info_(info)
{}

MixtCompFacade::~MixtCompFacade()
{
  if(p_model_!=NULL) {
    delete p_model_;
    p_model_ = NULL;
  }

  if(p_algo_!=NULL){
    delete p_algo_;
    p_algo_ = NULL;
  }

  if(p_strategy_!=NULL){
    delete p_strategy_;
    p_strategy_ = NULL;
  }

  if(p_mixture_){
    delete p_mixture_;
    p_mixture_ = NULL;
  }

  //Release memory of data handler
  DataHandler* p_datahandler = DataHandler::getInstance();
  if(p_datahandler!=NULL) delete p_datahandler;
}

bool MixtCompFacade::instantiateFramework(){
  //read data and set number of samples
  DataHandler* datainstance = DataHandler::getInstance();
  if(!datainstance->readDataFromFile(info_.datafilename_,info_.filesep_))
  {
    return false;
  }

  if(!datainstance->readModalityFromFile(info_.modalitiesfilename_,info_.filesep_)){
    return false;
  }

  info_.nbSample_ = datainstance->nbSamples();

  p_mixture_ = new CompositeMixture(v_mixture_);

  //create model
  p_model_ = new Model(p_mixture_,info_.nbSample_,info_.nbCluster_);

  //create algorithm
  p_algo_ = new SEMGibbs();

  //create strategy
  p_strategy_ = new IterationsStrategy(info_.nbIterations_,info_.burnin_,info_.nbtry_);

  return true;
}

bool MixtCompFacade::registerMixture(IMixture& mixture){
  v_mixture_.push_back(&mixture);
}

void MixtCompFacade::run(){
  //Run the algorithm using strategy on statistical model
  p_strategy_->run(p_algo_,p_model_);

  //copy parameters
  p_mixture_->copy(p_model_->getMixture());
  const CompositeMixture* temp = static_cast<const CompositeMixture*>(p_mixture_);
  for (int i = 0; i < v_mixture_.size(); ++i) {
    v_mixture_[i]->copy(*temp->getComponents()[i]);
  }
}
