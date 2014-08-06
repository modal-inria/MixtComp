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
 *  Created on: Ap 14, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 *              iovleff <S..._Dot_I..._At_stkpp_Dot_org> (see copyright for ...)
 **/

/** @file mixt_InitializeMixtureImpl.h
 *  @brief In this file .
 **/

#ifndef MIXT_INITIALIZEMIXTUREIMPL_H
#define MIXT_INITIALIZEMIXTUREIMPL_H

namespace mixt
{

/** Initialize mixture, default implementation */
template<int Id>
struct InitializeMixtureImpl
{
  typedef typename BridgeTraits<Id>::Mixture Mixture;
  typedef typename BridgeTraits<Id>::AugData AugData;
  static void run(Mixture& mixture, AugData& augData)
  {
    mixture.setData(augData.data_);
    mixture.initializeModel();
  }
};

/** Initialize mixture, specialization for Categorical_pjk models
 **/
template<>
struct InitializeMixtureImpl<STK::Clust::Categorical_pjk_>
{
  typedef typename BridgeTraits<STK::Clust::Categorical_pjk_>::Mixture Mixture;
  typedef typename BridgeTraits<STK::Clust::Categorical_pjk_>::AugData AugData;
  static void run(Mixture& mixture, AugData& augData)
  {
    mixture.setData(augData.data_);

    // compute the global min and max for all variables in current mixture, as in CategoricalDataStat::setModalities()
    std::vector<Range<int> >::iterator it   (augData.dataRanges_.begin());
    std::vector<Range<int> >::iterator itEnd(augData.dataRanges_.end  ());

    int min = it->min_;
    int max = it->max_;

    ++it;

    for (; it != itEnd; ++it)
    {
      int currMin = it->min_;
      int currMax = it->max_;
      if (currMin < min) min = currMin;
      if (currMax > max) max = currMax;
    }

    mixture.setModalities(STK::Range(min, max - min +1));
    // TODO: resize proba_ in initializeModel
    mixture.initializeModel();
  }
};

} // namespace mixt

#endif /* MIXT_INITIALIZEMIXTUREIMPL_H */
