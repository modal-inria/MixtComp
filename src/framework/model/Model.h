#ifndef MODEL_H_
#define MODEL_H_
class IDeveloper;
class Model
{
  public:
    Model(IDeveloper* developer,int nbsample,int nbcluster);
    Model(const Model*);
    void initializeModel();
    void eStep(){/* throw exception*/};
    void seStep();
    void ceStep(){/* throw exception*/};
    void mStep();
    void storeIntermediateResults(int iteration);
    void finalizeModel();
    void setDeveloper(IDeveloper*);
    double logLikelihood() const;
    int nbCluster() const {return nbCluster_;}
    int nbSample() const {return nbSample_;}
    double** conditionalProbabilities(){return m_Tik_;}
    int* classLabels(){return v_Zi_;}
    double* proportions(){return v_Pie_;}
    /** overloaded assignment operator*/
    Model& operator=(const Model&);
    ~Model();
  protected:
    IDeveloper *p_developer_;
    double **m_Tik_;
    int* v_Zi_;
    double* v_Pie_;
    int nbSample_;
    int nbCluster_;

    //protected member functions
    void updateModelParameters();
    void randomInitialization();
};


inline void Model::setDeveloper(IDeveloper * developer)
{
  p_developer_ = developer;
}
#endif /* MODEL_H_ */
