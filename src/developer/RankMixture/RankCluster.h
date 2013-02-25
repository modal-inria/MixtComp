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
    virtual void initializeStep();
    virtual void imputationStep();
    virtual void samplingStep();
    virtual void paramUpdateStep();
    virtual void finalizeStep();
    virtual double posteriorProbabilty();
    virtual double** allPosteriorProbabilties();
    virtual double logLikelihood() const;
    virtual int freeParameters() const;
    virtual void writeParameters(std::ostream&) const;
    virtual void setData();
    virtual ~RankCluster();
  protected: //or private
    //you can write here function for your own
  private:
    std::vector<int> m_;
    //write your other parameter here
};

#endif /* RANKCLUSTER_H_ */
