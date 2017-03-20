/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Mar 19, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 *              iovleff <S..._Dot_I..._At_stkpp_Dot_org> (see copyright for ...),
 *              Parmeet Bhatia <b..._DOT_p..._AT_gmail_Dot_com>
 **/


#ifndef MIXT_CLUST_TRAITS_H
#define MIXT_CLUST_TRAITS_H

namespace mixt
{

/** Given the Id of a mixture model, the struct Traits allow to get
 *  type of the input and output data of the parameters. This Traits class is
 *  used by the Bridge classes.
 *  @sa MixtureBridge
 *   **/
template<int id> struct BridgeTraits;

}  // namespace mixt

#endif /* MIXT_CLUST_TRAITS_H */
