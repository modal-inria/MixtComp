#include "framework/DataHandling/DataHandler.h"
#include "MixtCompFacade.h"

MixtCompFacade::MixtCompFacade(FrameworkInfo& info): info_(info)
{}

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

  if(p_mixture_){
    delete p_mixture_;
    p_mixture_ = NULL;
  }

  for (int i = 0; i < v_mixture_.size(); ++i) {
    if(v_mixture_[i]) {
      delete v_mixture_[i];
      v_mixture_[i] = NULL;
    }
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

bool MixtCompFacade::registerLaw(IMixture& mixture){
  v_mixture_.push_back(&mixture);
}

void MixtCompFacade::run(){
  //Run the algorithm using strategy on statistical model
  p_strategy_->run(p_algo_,p_model_);
  //p_mixture_ now have all the estimated parameters and result
  /*************************************************************/
  //print parameters to console
  p_model_->writeParameters(std::cout);
}
