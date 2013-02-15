#ifndef RANKCLUSTER_H_
#define RANKCLUSTER_H_
/**@file RankCluster.h
 * @brief 
 */
#include <vector>
#include "framework/DeveloperInterface/IDeveloper.h"

// create your own data structures
struct Rank{
    //write your Rank params
};


class RankCluster : public IDeveloper
{
  public:
    RankCluster();
    virtual void initializeStep(double** Zik);
    virtual void imputationStep(double* Pk, double** Zik);
    virtual void samplingStep(double* Pk, double** Zik);
    virtual void paramUpdateStep(double* Pk, double** Zik);
    virtual void finalizeStep();
    virtual double posteriorProbabilty(int sample_num,int Cluster_num);
    virtual double** allPosteriorProbabilties();
    virtual double logLikelihood() const;
    virtual int freeParameters() const;
    virtual void setDataFromFile(std::string filename);
    virtual ~RankCluster();
  protected: //or private
    //you can write here function for your own
  private:
    std::vector<int> m_;
    //write your other parameter here
};

#endif /* RANKCLUSTER_H_ */
