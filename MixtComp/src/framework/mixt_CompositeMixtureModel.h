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

    virtual IMixtureModelBase* create() const = 0;
    virtual IMixtureModelBase* clone() const = 0;
    virtual bool randomInit() =0;
    virtual void mStep() = 0;
    virtual STK::Real lnComponentProbability(int i, int k);

    virtual void writeParameters(std::ostream& os) const;
    virtual void initializeModel();
    virtual void initializeStep();
    virtual void inputationStep();
    virtual void computeProportions();
    virtual void finalizeStep();

  private:
    std::vector<*IMixture> v_mixtures_;
};

} /* namespace mixt */
#endif /* MIXT_COMPOSITEMIXTURE_H_ */
