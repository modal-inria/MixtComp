#ifndef MIXTCOMPFACADE_H_
#define MIXTCOMPFACADE_H_
/**@file MixtCompFacade.h
 * @brief 
 */
#include <initializer_list>
#include "framework/DeveloperInterface/IDeveloper.h"
#include "framework/model/Model.h"
#include "framework/algorithms/IAlgo.h"
#include "framework/strategy/IStrategy.h"

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
  int iterations_;
};

class MixtCompFacade
{
  public:
    MixtCompFacade(FrameworkInfo);
    void run();
    void instantiateFramework();
    ~MixtCompFacade();
  protected:
    IDeveloper *p_developer_ = NULL;
    std::vector<IDeveloper*> v_developer_;
    IAlgo *p_algo_ = NULL;
    Model *p_model_ = NULL;
    IStrategy *p_strategy_ = NULL;
    FrameworkInfo info_;
};

inline void MixtCompFacade::run(){
  //Prepare and initialize the statistical model
  p_model_->initializeModel();
  //Run the algorithm using strategy on statistical model
  p_strategy_->run();
  //Finalize the statistical model
  p_model_->finalize();
  //p_developer_ now have all the estimated parameters and result
  /*************************************************************/

  //print parameters to console
  p_developer_->writeParameters(std::cout);
}

#endif /* MIXTCOMPFACADE_H_ */
