#ifndef ISTRATEGY_H_
#define ISTRATEGY_H_
/**@file IStrategy.h
 * @brief 
 */
#include "framework/algorithms/IAlgo.h"
#include "framework/model/Model.h"
class IStrategy
{
  public:
    IStrategy();
    void run() = 0;
    void setAlgo(IAlgo*);
    void setModel(Model*);
    ~IStrategy();
  protected:
    IAlgo *p_algo_;
    Model *p_model_;
};

inline void IStrategy::setAlgo(IAlgo *algo){
  p_algo_ = algo;
}

inline void IStrategy::setModel(Model *model){
  p_model_ = model;
}
#endif /* ISTRATEGY_H_ */
