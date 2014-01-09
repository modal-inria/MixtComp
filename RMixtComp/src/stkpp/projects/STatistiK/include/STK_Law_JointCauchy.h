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
 * Project:  stkpp::STatistiK::Law
 * created on: 23 janv. 2013
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Law_JointCauchy.h
 *  @brief In this file we define the joint Cauchy probability law.
 **/

#ifndef STK_LAW_JOINTCAUCHY_H
#define STK_LAW_JOINTCAUCHY_H

#include "STK_Law_IMultiLaw.h"
#include "STK_Law_Cauchy.h"

namespace STK
{

namespace Law
{
/** @ingroup Laws
 * @brief Implement the Joint Cauchy probability law
 **/
template<class RowVector>
class JointCauchy: public JointProbability<RowVector, Cauchy>
{
  public:
    typedef JointProbability<RowVector, Cauchy> Base;
    /** default constructor */
    JointCauchy() : Base(_T("JointCauchy")) {}
    /** constructor */
    JointCauchy( int const& nbVar)
               : Base(_T("JointCauchy"), nbVar) {}
    /** destructor */
    virtual ~JointCauchy() {}
    /** @return the location of the jth law */
    inline Real const& location(int const& j) const { return this->law(j).location();}
    /** @return the scale of the jth law */
    inline Real const& scale(int const& j) const { return this->law(j).scale();}
    /** set the location of the jth law */
    inline void setLocation(int const& j, Real const& location)
    { this->law(j).setMu(location);}
    /** set the scale of the jth law */
    inline void setScale(int const& j, Real const& scale)
    { this->law(j).setScale(scale);}
};

} // namespace Law

} // namespace STK

#endif /* STK_LAW_JOINTCAUCHY_H */
