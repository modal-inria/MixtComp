#ifndef MIXTCOMPFACADE_H_
#define MIXTCOMPFACADE_H_
/**@file MixtCompFacade.h
 * @brief 
 */
#include <initializer_list>
#include <iostream>
#include "framework/DeveloperInterface/IDeveloper.h"
#include "framework/DeveloperInterface/CompositeDeveloper.h"
#include "framework/model/Model.h"
#include "framework/algorithms/IAlgo.h"
#include "framework/algorithms/SEMGibbs.h"
#include "framework/strategy/IStrategy.h"
#include "framework/strategy/IterationsStrategy.h"

enum MixtureLaw{
    gaussian_,
    binary_,
    multinomial_,
    rank_
};

enum Algo{
    semgibbs_
};

enum strategy{
    iterations_
};

struct FrameworkInfo{
  std::initializer_list<MixtureLaw> mixturelawlist_;
  Algo algorithm_;
  strategy strategy_;
  int nbIterations_;
  int burnin_;
  int nbtry_;
  int nbSample_;
  int nbCluster_;
};

class MixtCompFacade
{
  public:
    MixtCompFacade(FrameworkInfo& info);
    void run();
    void instantiateFramework();
    ~MixtCompFacade();
  protected:
    IDeveloper *p_developer_;
    std::vector<IDeveloper*> v_developer_;
    IAlgo *p_algo_;
    Model *p_model_;
    IStrategy *p_strategy_;
    FrameworkInfo info_;
};

inline void MixtCompFacade::run(){
  //Run the algorithm using strategy on statistical model
  p_strategy_->run(p_algo_,p_model_);
  //p_developer_ now have all the estimated parameters and result
  /*************************************************************/
  //print parameters to console
  p_developer_->writeParameters(std::cout);
}

#endif /* MIXTCOMPFACADE_H_ */
