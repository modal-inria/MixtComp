#ifndef IALGO_H_
#define IALGO_H_
/**@file IAlgo.h
 * @brief 
 */
#include "model/Model.h"
class IAlgo
{
  public:
    IAlgo();
    virtual void run() const = 0;
    virtual ~IAlgo();
  protected:
    Model * p_model_;
};

#endif /* IALGO_H_ */
