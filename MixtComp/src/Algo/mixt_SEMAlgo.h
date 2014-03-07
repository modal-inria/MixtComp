/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2014

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
 *  Created on: Mar 7, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 *              Parmeet Bhatia <b..._DOT_p..._AT_gmail_Dot_com>
 **/

#ifndef MIXT_MIXTUREALGO_H
#define MIXT_MIXTUREALGO_H

#include "../Composer/mixt_MixtureComposer.h"

namespace mixt
{
// forward declaration
class IMixtureComposerBase;

/** @ingroup Clustering
 * Interface base class for the algorithms.
 * All algorithms are runners applying on a model instance given by pointer
 * and have to implement the run method.
 *
 * All algorithms start with an mStep(), so user have to provide an instance of
 * the model with initial parameters values.
 **/
class SEMAlgo
{
  public:
    /** default constructor */
    SEMAlgo(int nbIterMax, int zMin, int nbSamplingAttempts);
    /** Copy constructor.
     *  @param algo the algorithm to copy */
    SEMAlgo(SEMAlgo const& algo);
    /** destructor */
    virtual ~SEMAlgo();
    /** set a new model */
    void setModel(MixtureComposer* p_model);
    /** run the algorithm */
    bool run();
  private:
    /** pointer on the mixture model */
    MixtureComposer* p_model_;
    /** number of iterations of the algorithm */
    int nbIterMax_;
    /** minimal number of elements per class */
    int zMin_;
    /** number of attempts before giving up on sampling */
    int nbSamplingAttempts_;
};

} // namespace mixt

#endif /* MIXT_MIXTUREALGO_H */
