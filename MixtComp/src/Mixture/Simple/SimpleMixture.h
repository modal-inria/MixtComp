/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: 15th of January, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef SIMPLEMIXTURE_H
#define SIMPLEMIXTURE_H

#include "Mixture/mixt_IMixture.h"
#include "Data/mixt_AugmentedData.h"

namespace mixt {

// TODO: add the complete template types for DataHandler & Co

template<typename Model>
// class SimpleLegacyBridge : public IMixture {
class SimpleMixture {

public:
private:
protected:
	/** The augmented data set */
	AugmentedData<typename Model::Data> augData_;

	/** The simple mixture to bridge with the composer */
	Model model_;
};

}

#endif /* MIXT_MIXTUREBRIDGE_H */
