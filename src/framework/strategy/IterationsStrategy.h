#ifndef ITERATIONSSTRATEGY_H_
#define ITERATIONSSTRATEGY_H_
/**@file IterationsStrategy.h
 * @brief 
 */
#include "IStrategy.h"
class IterationsStrategy : public IStrategy
{
  public:
    IterationsStrategy(int,int,int);
    virtual void run(IAlgo*,Model*&);
    virtual ~IterationsStrategy();
  protected:
    int iterations_,burnin_,nbtry_;
};

#endif /* ITERATIONSSTRATEGY_H_ */
