#ifndef SEMGIBBS_H_
#define SEMGIBBS_H_
/**@file SEMGibbs.h
 * @brief 
 */
class IAlgo;
class SEMGibbs : public IAlgo
{
  public:
    SEMGibbs();
    virtual void run();
    virtual ~SEMGibbs();
};

#endif /* SEMGIBBS_H_ */
