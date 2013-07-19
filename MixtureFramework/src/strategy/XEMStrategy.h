#ifndef XEMSTRATEGY_H_
#define XEMSTRATEGY_H_
/**@file XEMStrategy.h
 * @brief 
 */
#include "IStrategy.h"
#include "../model/IModel.h"
#include "../algorithm/IAlgo.h"
#include "../initialization/IInit.h"

struct XEMstratParam{
    int nbShortruns_;
    int nbLongruns_;
    int nbShortIterations_;
    int nbLongIterations_;
    double epsilonShort_;
    double epsilonLong_;
};

class XEMStrategy:public IStrategy
{
  public:
    XEMStrategy(XEMstratParam&);
    virtual void run(IModel*&,IAlgo*&,IInit*&);
    virtual ~XEMStrategy();
  protected:
    XEMstratParam param_;
};

#endif /* XEMSTRATEGY_H_ */
