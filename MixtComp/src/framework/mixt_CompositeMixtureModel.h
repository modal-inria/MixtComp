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

namespace mixt
{

class CompositeMixtureModel : public STK::IMixtureModelBase
{
  public:
    CompositeMixtureModel(int nbCluster);
    virtual ~CompositeMixtureModel();

    /** Create a clone of the current model, but reinitialize the ingredients parameters. */
    virtual IMixtureModelBase* create();
    /** Create a clone of the current model, with ingredients parameters preserved. */
    virtual IMixtureModelBase* clone();

    virtual void mStep();
    virtual STK::Real lnComponentProbability(int i, int k);

    virtual void writeParameters(std::ostream& os) const;
    virtual void initializeModel();
    virtual void initializeStep();
    virtual void imputationStep();
    virtual void finalizeStep();

  private:
    std::vector<*IMixture> v_mixtures_;
    /** randomInit is currently disabled and hidden, pending future developments */
    virtual bool randomInit();
    /** In which case should the proportions computed differently, considering a composite mixture ? */
    virtual void computeProportions();
};

} /* namespace mixt */
#endif /* MIXT_COMPOSITEMIXTURE_H_ */
