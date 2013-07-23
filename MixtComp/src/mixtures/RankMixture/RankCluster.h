#ifndef RANKCLUSTER_H_
#define RANKCLUSTER_H_

/**@file RankCluster.h
 * @brief
 */
#include <vector>
#include <set>
//#include "../../../../Eigen/Eigen/Dense"
#include "framework/MixtureInterface/IMixture.h"
#include "stkpp/include/STKpp.h"


#include "functions.h"
using namespace std;
// create your own data structures
struct PartialRank
{
    std::vector<int> rank;
    std::vector<int> y;
    bool isPartial;
    std::set<int> missingData;
    std::vector<int> missingIndex;

};

struct SEMparameters
{
  std::vector<int> nGibbsSE;
  std::vector<int> nGibbsM;
  int maxIt;
  int burnAlgo;
  int nGibbsL;
  int burnL;
  bool detail;
};

struct OutParameters
{
  double L;
  double bic;
  double icl;
  STK::Array2D<double> tik;

  //algorithm initialization
  std::vector<std::vector<std::vector<int> > > initialPartialRank;
  std::vector<std::vector<double> > initialP;
  std::vector<int> initialZ;
  std::vector<std::vector<std::vector<int> > > initialMu;

  //distance between parameters
  std::vector<std::vector<double> > distProp;
  std::vector<std::vector<std::vector<double> > > distP;
  std::vector<std::vector<std::vector<int> > > distMu;
  std::vector<double> distZ;
  std::vector<std::vector<std::vector<int> > > distPartialRank;

};

class RankCluster: public IMixture
{
  public:
    RankCluster();
    RankCluster(char id,int iterations,int burnin);
    virtual RankCluster* clone();
    virtual void copy(const IMixture&);
    virtual void initializeStep();
    virtual void imputationStep() {/**Do nothing by default*/}
    virtual void samplingStep();
    virtual void paramUpdateStep();
    virtual void storeIntermediateResults(int iteration);
    virtual void finalizeStep() {/**Do nothing by default*/}
    virtual double posteriorProbability(int sample_num,int Cluster_num);
    virtual double logLikelihood();
    virtual int freeParameters() const;
    virtual void setData();
    virtual void writeParameters(std::ostream&) const;
    virtual ~RankCluster();

  protected: //or private
    void conversion2data(STK::Array2D<int> const& X);
    void initialization();
    void SEstep();
    void gibbsY(int indexDim);
    void zSimulation();
    void gibbsX(int indexDim);
    void Mstep();
    void simuM(int indexDim,int indCl);
    void likelihood(std::vector<std::vector<std::vector<std::vector<int> > > > &listeMu,std::vector<std::vector<std::vector<double> > > &resP,
            std::vector<std::vector<double> > &resProp);
    double computeLikelihood(std::vector<std::vector<std::vector<int> > > const& mu,std::vector<std::vector<double> > const& p,
        std::vector<double> const& proportion,STK::Array2D<double> &tik,std::vector<std::vector<std::vector<int> > > &Y,
        std::vector<std::vector<std::vector<int> > > &xTemp);
    void computeDistance(std::vector<std::vector<double> > const& resProp,std::vector<std::vector<std::vector<double> > > const& resP,
        std::vector<std::vector<std::vector<std::vector<int> > > > const& resMu,std::vector<std::vector<int> > const& resZ,
        std::vector<std::vector<std::vector<std::vector<int> > > > const& resDonneesPartiel);

    //parameters
    std::vector<int> m_;//contains the size of rank for each dim
    int n_;//number of individuals
    int d_;//number of dimension
    int g_;//number of cluster
    std::vector<std::vector<PartialRank> > data_;
    std::vector<int> z_;
    std::vector<std::vector<std::vector<int> > > mu_;// mu_[dimension][cluster][indice]
    std::vector<std::vector<double> > p_;// p_[dimension][cluster]
    std::vector<double> proportion_;
    SEMparameters parameter_;
    OutParameters output_;
    bool partial_;//true if there is partial rank in the data
    std::vector<std::vector<int> > indexPartialData_;//index of partial data

    //objet pour stocker les resultats des itérations
    vector<int> indrang;
    vector<vector<vector<double> > > resP;
    vector<vector<double> > resProp;
    vector<vector<int> > resZ;
    vector<vector<vector<vector<int> > > > resMu;
    vector<vector<vector<vector<int> > > > resDonneesPartiel;


};

template <typename T>
std::ostream& operator<<( std::ostream &flux, std::vector<T> vect)
{
    for (unsigned int i(0);i<vect.size();i++)
        flux << " " << vect[i];
    flux<<std::endl;
    return flux;
}
#endif /* RANKCLUSTER_H_ */
