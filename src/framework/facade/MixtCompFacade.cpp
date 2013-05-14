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
  //read data and set number of samples
  DataHandler* datainstance = DataHandler::getInstance();
  datainstance->readDataFromFile(info_.datafilename_,' ');
  datainstance->readModalityFromFile(info_.modalitiesfilename_,' ');
  info_.nbSample_ = datainstance->nbSamples();
  //TODO getting information from data which models to instantiate

  //TODO creation of v_developer_
  for(MixtureLaw law: info_.mixturelawlist_){
    switch (law) {
      case rank_:
        v_developer_.push_back(new RankCluster('R',info_.nbIterations_,info_.burnin_));
        break;
      case gaussian_:
        v_developer_.push_back(new gaussianMixture());
        break;
      default:
        break;
    }
  }

  p_developer_ = new CompositeDeveloper(v_developer_);
  //create model
  p_model_ = new Model(p_developer_,info_.nbSample_,info_.nbCluster_);

  //create algorithm
  switch (info_.algorithm_) {
    case semgibbs_:
      p_algo_ = new SEMGibbs();
      break;
    default:
      p_algo_ = new SEMGibbs();
      break;
  }
  //create strategy
  switch (info_.strategy_) {
    case iterations_:
      p_strategy_ = new IterationsStrategy(info_.nbIterations_,info_.burnin_,info_.nbtry_);
      break;
    default:
      p_strategy_ = new IterationsStrategy(info_.nbIterations_,info_.burnin_,info_.nbtry_);
      break;
  }
}

void MixtCompFacade::run(){
  //Run the algorithm using strategy on statistical model
  p_strategy_->run(p_algo_,p_model_);
  //p_developer_ now have all the estimated parameters and result
  /*************************************************************/
  //print parameters to console
  p_model_->writeParameters(std::cout);
}
