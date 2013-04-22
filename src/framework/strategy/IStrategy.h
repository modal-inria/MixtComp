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
    virtual void run(IAlgo*,Model*) = 0;
    virtual ~IStrategy();
};
#endif /* ISTRATEGY_H_ */
