/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2013-2014

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
template<int id> struct MixtureTraits;

}  // namespace mixt

#endif /* MIXT_CLUST_TRAITS_H */
