#ifndef IMODEL_H_
#define IMODEL_H_
/**@file IModel.h
 * @brief 
 */
class IModel
{
  public:
    IModel();
    virtual IModel* create() = 0;
    virtual IModel* clone() = 0;
    virtual IModel& operator=(const IModel&) = 0;
    virtual void eStep() = 0;
    virtual void ceStep() = 0;
    virtual void seStep() = 0;
    virtual void mStep() = 0;
    virtual bool cemInit(){};
    virtual bool fuzzycemInit(){};
    virtual bool randomInit(){};
    virtual double lnLikelihood() = 0;
    virtual ~IModel();
};

#endif /* IMODEL_H_ */
