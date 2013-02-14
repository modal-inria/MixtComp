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

 Contact : Serge.Iovleff@stkpp.org
 */

/*
 * Project:  MixtComp::
 * created on: 14 févr. 2013
 * Author:   iovleff, serge.iovleff@stkpp.org
 **/

/** @file BernouilliMixture.h
 *  @brief In this file .
 **/

#ifndef BERNOUILLIMIXTURE_H_
#define BERNOUILLIMIXTURE_H_

/** @brief
 *
 */
class BernouilliMixture: public MixtureBridge<STK::JointBernoulliModel<STK::Array2D<STK::Binary> > >
{
  public:
    BernouilliMixture()
    { }
    virtual ~BernouilliMixture()
    {
    }
    virtual void writeParameters(std::ostream& os);

};

#endif /* BERNOUILLIMIXTURE_H_ */
