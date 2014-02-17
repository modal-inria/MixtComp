/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013  Serge Iovleff

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as
 published by the Free Software Foundation; either version 2 of the
 License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this program; if not, write to the
 Free Software Foundation, Inc.,
 59 Temple Place,
 Suite 330,
 Boston, MA 02111-1307
 USA

 Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 */

/*
 * Project:  stkpp::
 * created on: 23 janv. 2013
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Law_JointBernoulli.h
 *  @brief In this file .
 **/

#ifndef STK_LAW_JOINTBERNOULLI_H
#define STK_LAW_JOINTBERNOULLI_H

#include "STK_Law_IMultiLaw.h"
#include "STK_Law_Bernoulli.h"

namespace STK
{

namespace Law
{
/** @ingroup Laws
 * @brief Implement the Joint Bernoulli probability law
 **/
template<class RowVector>
class JointBernoulli: public JointProbability<RowVector, Bernoulli>
{
  public:
    typedef JointProbability<RowVector, Bernoulli> Base;
    /** default constructor */
    JointBernoulli() : Base(_T("JointBernoulli")) {}
    /** constructor */
    JointBernoulli( int const& nbVar)
                  : Base(_T("JointBernoulli"), nbVar) {}
    /** destructor */
    virtual ~JointBernoulli() {}
    /** @return the probability of success of the jth law */
    inline Real const& prob(int const& j) const
    { return this->law(j).prob();}
    /** @return the probability of success of the jth law */
    inline void setProb(int const& j, Real const& prob)
    { this->law(j).setProb(prob);}
};

} // namespace Law

} // namespace STK

#endif /* STK_LAW_JOINTBERNOULLI_H */
