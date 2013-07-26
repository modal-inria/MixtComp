#ifndef INCLUDE_H_
#define INCLUDE_H_
/**@file include.h
* @brief 
*/

#include "model/IModel.h"
#include "concretemodels/StkppMixtures/GaussianMixture.h"

#include "algorithm/IAlgo.h"
#include "algorithm/EMAlgo.h"
#include "algorithm/CEMAlgo.h"
#include "algorithm/SEMAlgo.h"

#include "strategy/IStrategy.h"
#include "strategy/SimpleStrategy.h"
#include "strategy/XEMStrategy.h"

#include "initialization/IInit.h"
#include "initialization/CEMInit.h"
#include "initialization/FuzzyCEMInit.h"
#include "initialization/RandomInit.h"

#include "facade/facade.h"

#endif /* INCLUDE_H_ */
