#ifndef MODEL_H_
#define MODEL_H_
/* Forward Declaration */
class IMixtureLaw;
class Model
{
  public:
    Model();
    //Various Algorithm steps
    virtual void Estep();
    virtual void Sstep();
    virtual void Mstep();
    //various Initialization steps
    virtual void RandomInit();
    virtual ~Model();
  protected:
    IMixtureLaw * p_MixtureLaw_;
    MatrixReal m_Tik_, m_Cik, m_Zik;
    ColVectorReal v_Pie;
};

#endif /* MODEL_H_ */
