#ifndef FACADE_H_
#define FACADE_H_
/**@file facade.h
 * @brief 
 */
#include "strategy/IStrategy.h"
class IModel;
class IAlgo;
class IInit;
class facade
{
  public:
    facade();
    void run();
    inline void setAlgo(IAlgo*& algo) {p_Algo_ = algo;}
    inline void setModel(IModel*& model) {p_Model_ = model;}
    inline void setStratgy(IStrategy*& strategy) {p_Strategy_ = strategy;}
    inline void setInit(IInit*& init) {p_Init_ = init;}
    virtual ~facade();
  protected:
    IModel* p_Model_;
    IStrategy* p_Strategy_;
    IAlgo* p_Algo_;
    IInit* p_Init_;
};

inline void facade::run(){
  p_Strategy_->run(p_Model_,p_Algo_,p_Init_);
}

#endif /* FACADE_H_ */
