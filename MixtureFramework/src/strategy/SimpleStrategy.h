#ifndef SIMPLESTRATEGY_H_
#define SIMPLESTRATEGY_H_
/**@file SimpleStrategy.h
 * @brief 
 */
#include "IStrategy.h"
#include "model/IModel.h"
#include "algorithm/IAlgo.h"
#include "initialization/IInit.h"

struct SimpleStratParam{
    int nbTry_;
    int nbIterations_;
    double epsilon_;
};


class SimpleStrategy:public IStrategy
{
  public:
    SimpleStrategy();
    virtual void run(IModel*&,IAlgo*&,IInit*&);
    virtual ~SimpleStrategy();
  protected:
    SimpleStratParam param_;
};

#endif /* SIMPLESTRATEGY_H_ */
