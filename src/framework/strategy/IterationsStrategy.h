#ifndef ITERATIONSSTRATEGY_H_
#define ITERATIONSSTRATEGY_H_
/**@file IterationsStrategy.h
 * @brief 
 */
#include "IStrategy.h"
class IterationsStrategy : public IStrategy
{
  public:
    IterationsStrategy(int);
    virtual void run();
    virtual ~IterationsStrategy();
  protected:
    int iterations_;
};

#endif /* ITERATIONSSTRATEGY_H_ */
