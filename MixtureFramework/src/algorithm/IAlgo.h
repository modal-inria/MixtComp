#ifndef IALGO_H_
#define IALGO_H_
/**@file IAlgo.h
 * @brief 
 */
class IModel;

class IAlgo
{
  public:
    IAlgo();
    virtual void run(IModel*&) = 0;
    virtual ~IAlgo();
};

#endif /* IALGO_H_ */
