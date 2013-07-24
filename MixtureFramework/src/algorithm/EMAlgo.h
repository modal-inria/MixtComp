#ifndef EMALGO_H_
#define EMALGO_H_
/**@file EMAlgo.h
 * @brief 
 */
#include "IAlgo.h"
#include "model/IModel.h"
class EMAlgo:public IAlgo
{
  public:
    EMAlgo();
    virtual void run(IModel*&);
    virtual ~EMAlgo();
};

#endif /* EMALGO_H_ */
