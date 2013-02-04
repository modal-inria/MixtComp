#ifndef MODEL_H_
#define MODEL_H_
/* Forward Declaration */
class IMixtureLaw;
class Model
{
  public:
    Model();
    //Various Algorithm steps
    virtual void EStep();
    virtual void SEStep();
    virtual void CEStep();
    virtual void MStep();
    //various Initialization steps
    virtual void EMInit();
    virtual void CEMInit();
    virtual void SEMInit();
    virtual void RandomInit();
    virtual ~Model();
  protected:
    IMixtureLaw * p_MixtureLaw_;

};

#endif /* MODEL_H_ */
