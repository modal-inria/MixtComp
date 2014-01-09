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

/** @file STK_Law_JointGamma.h
 *  @brief In this file we define the joint Gamma probability law.
 **/

#ifndef STK_LAW_JOINTGAMMA_H
#define STK_LAW_JOINTGAMMA_H

#include "STK_Law_IMultiLaw.h"
#include "STK_Law_Gamma.h"

namespace STK
{

namespace Law
{
/** @ingroup Laws
 * @brief Implement the Joint Gamma probability law
 **/
template<class RowVector>
class JointGamma: public JointProbability<RowVector, Gamma>
{
  public:
    typedef JointProbability<RowVector, Gamma> Base;
    /** default constructor */
    JointGamma() : Base(_T("JointGamma")) {}
    /** constructor */
    JointGamma( int const& nbVar) : Base(_T("JointGamma"), nbVar) {}
    /** destructor */
    virtual ~JointGamma() {}
    /** @return the shape of the jth law */
    inline Real const& shape(int const& j) const { return this->law(j).shape();}
    /** set the shape of the jth law */
    inline void setShape(int const& j, Real const& shape)
    { this->law(j).setShape(shape);}
    /** @return the scale of the jth law */
    inline Real const& scale(int const& j) const { return this->law(j).scale();}
    /** set the scale of the jth law */
    inline void setScale(int const& j, Real const& scale)
    { this->law(j).setScale(scale);}
};

} // namespace Law

} // namespace STK

#endif /* STK_LAW_JOINTNORMAL_H */
