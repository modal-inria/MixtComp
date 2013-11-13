/*
 * mixt_CompositeMixture.h
 *
 *  Created on: 16 oct. 2013
 *      Author: iovleff
 */

#ifndef MIXT_COMPOSITEMIXTURE_H
#define MIXT_COMPOSITEMIXTURE_H

#include <vector>
#include "../stkpp/include/Clustering.h"
#include "../mixtureInterface/IMixture.h"
#include "../dataHandling/mixt_DataHandler.h"

namespace mixt
{

class CompositeMixtureModel : public STK::IMixtureModelBase
{
  public:
    CompositeMixtureModel(DataHandler* p_dataHandler, int nbCluster);
    CompositeMixtureModel(DataHandler* p_dataHandler, int nbCluster, int nbSamples);
    CompositeMixtureModel(CompositeMixtureModel const& model);

    /**
     * Note that the CompositeMixtureModel destructor does not destroy the
     * individual IMixture objets pointed to by v_mixtures_ .
     * Manual destruction is therefore expected to avoid memory leaks.
     */
    virtual ~CompositeMixtureModel();

    /** Create a clone of the current model, but reinitialize the ingredients parameters. */
    virtual CompositeMixtureModel* create() const;
    /** Create a clone of the current model, with ingredients parameters preserved. */
    virtual CompositeMixtureModel* clone() const;

    virtual void mStep();
    virtual STK::Real lnComponentProbability(int i, int k);

    virtual void writeParameters(std::ostream& os) const;
    virtual void initializeModel();
    virtual void initializeStep();
    virtual void imputationStep();
    virtual void finalizeStep();
    virtual void registerMixture(IMixture* mixture);
    template<typename Data>
    inline void getData(Data& data) const { p_dataHandler_->getData<Data>(data);}

  private:
    /** pointer to the dataHandler */
    DataHandler const* p_dataHandler_;
    /** vector of pointers to the mixtures components */
    std::vector<IMixture*> v_mixtures_;
    /** randomInit is currently disabled and hidden, pending future developments */
    virtual void randomInit();
    /** In which case should the proportions be computed differently, considering a composite mixture ? */
    virtual void computeProportions();
    /** compute the number of free parameters of the model. */
    virtual int computeNbFreeParameters() const;
};

} /* namespace mixt */
#endif /* MIXT_COMPOSITEMIXTURE_H_ */
