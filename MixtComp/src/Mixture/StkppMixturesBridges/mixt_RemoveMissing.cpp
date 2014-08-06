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
 *  Created on: Nov 15, 2013
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 *              iovleff, serge.iovleff@stkpp.org
 **/

#include "mixt_RemoveMissing.h"

namespace mixt
{

/** Utility function to lookup the data set and remove missing values
 *  coordinates. */
void removeMissing(AugmentedData<STK::Array2D<STK::Real> >& m_augDataij)
{
  typedef typename std::pair<int, int> pos;

#ifdef MC_DEBUG
  std::cout << "removeMissing AugmentedData<STK::Array2D<STK::Real> >& " << std::endl
            << "m_augDataij.v_missing_.size(): " << m_augDataij.v_missing_.size() << std::endl
            << "m_augDataij.v_missingIntervals_.size(): " << m_augDataij.v_missingIntervals_.size() << std::endl
            << "m_augDataij.v_missingLUIntervals_.size(): " << m_augDataij.v_missingLUIntervals_.size() << std::endl
            << "m_augDataij.v_missingRUIntervals_.size(): " << m_augDataij.v_missingRUIntervals_.size() << std::endl;
#endif

  // missing value [-inf,+inf] or ?
  for (typename std::vector<pos>::iterator it = m_augDataij.v_missing_.begin();
       it != m_augDataij.v_missing_.end();
       ++it)
  {
    m_augDataij.data_((*it).first,
                       (*it).second) = STK::Law::Uniform::rand(m_augDataij.dataRanges_[(*it).second].min_,
                                                               m_augDataij.dataRanges_[(*it).second].max_);
  }

  // missing values [a,b]
  for (typename std::vector<std::pair<pos, std::pair<STK::Real, STK::Real> > >::iterator it = m_augDataij.v_missingIntervals_.begin();
       it != m_augDataij.v_missingIntervals_.end();
       ++it)
  {
    m_augDataij.data_((*it).first.first,
                       (*it).first.second) = STK::Law::Uniform::rand((*it).second.first,
                                                                     (*it).second.second);
  }

  // missing values [-inf,b]
  for (typename std::vector<std::pair<pos, STK::Real> >::iterator it = m_augDataij.v_missingLUIntervals_.begin();
       it != m_augDataij.v_missingLUIntervals_.end();
       ++it)
  {
    m_augDataij.data_((*it).first.first,
                       (*it).first.second) = STK::Law::Uniform::rand(m_augDataij.dataRanges_[(*it).second].min_,
                                                                     (*it).second);
  }

  // missing values [a,+inf]
  for (typename std::vector<std::pair<pos, STK::Real> >::iterator it = m_augDataij.v_missingRUIntervals_.begin();
       it != m_augDataij.v_missingRUIntervals_.end();
       ++it)
  {
    m_augDataij.data_((*it).first.first,
                       (*it).first.second) = STK::Law::Uniform::rand((*it).second,
                                                                     m_augDataij.dataRanges_[(*it).second].max_);
  }
}

void removeMissing(AugmentedData<STK::Array2D<int> >& m_augDataij)
{
  typedef typename std::pair<int, int> pos;

  // missing value [-inf,+inf] or ?
  for (typename std::vector<pos>::iterator it = m_augDataij.v_missing_.begin();
       it != m_augDataij.v_missing_.end();
       ++it)
  {
    int nbModalities = m_augDataij.dataRanges_[(*it).second].range_;
    STK::Array2DVector<STK::Real> modalities(nbModalities, 1. / nbModalities);
    m_augDataij.data_((*it).first,
                       (*it).second) = STK::Law::Categorical::rand(modalities);
  }

  // missing values {}
  for (typename std::vector<std::pair<pos, std::vector<int> > >::iterator it = m_augDataij.v_missingFiniteValues_.begin();
       it != m_augDataij.v_missingFiniteValues_.end();
       ++it)
  {
    int nbModalities = m_augDataij.dataRanges_[(*it).first.second].range_;
    STK::Real proba = 1. / (*it).second.size();
    STK::Array2DVector<STK::Real> modalities(nbModalities, 0.);
    for(std::vector<int>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2)
      modalities[*it2] = proba;

    int sampledValue = STK::Law::Categorical::rand(modalities);
    m_augDataij.data_((*it).first.first,
                      (*it).first.second) = sampledValue;
#ifdef MC_DEBUG
    std::cout << "removeMissing, int, v_missingFiniteValues_" << std::endl;
    std::cout << "pos: " << (*it).first.first << " " << (*it).first.second << std::endl;
    std::cout << "number of present values: " << (*it).second.size() << std::endl;
    std::cout << "nbModalities: " << nbModalities << std::endl;
    std::cout << "modalities: " << std::endl;
    std::cout << modalities;
    std::cout << "sampled value: " << sampledValue << std::endl;
#endif
  }
}

} /* namespace mixt */
