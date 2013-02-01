#ifndef MODEL_H_
#define MODEL_H_
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
};

#endif /* MODEL_H_ */
