#ifndef SEMALGO_H_
#define SEMALGO_H_
/**@file SEMAlgo.h
 * @brief 
 */
#include "IAlgo.h"
#include "model/IModel.h"

class SEMAlgo:public IAlgo
{
  public:
    SEMAlgo();
    virtual void run(IModel*&);
    virtual ~SEMAlgo();
};

#endif /* SEMALGO_H_ */
