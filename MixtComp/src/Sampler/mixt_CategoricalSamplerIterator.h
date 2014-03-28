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
 *  Created on: Mar 27, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_GAUSSIANSAMPLERITERATOR_H
#define MIXT_GAUSSIANSAMPLERITERATOR_H

#include "stkpp/projects/Arrays/include/STK_Array2D.h"
#include "stkpp/projects/Arrays/include/STK_CArrayVector.h"
#include "../Data/mixt_AugmentedData.h"

namespace mixt
{

typedef std::pair<int, int> pos;
typedef typename std::vector<          pos                        >::const_iterator iv_missing;
typedef typename std::vector<std::pair<pos, std::vector<int>    > >::const_iterator iv_missingFiniteValues;
typedef typename std::pair<pos, int> RetValue;

class CategoricalSamplerIterator
{
  public:
    CategoricalSamplerIterator(const STK::Array2D<int>* p_param,
                               const STK::CArrayVector<int>* p_zi,
                               const std::vector<std::pair<int, int> >& dataRange,
                               iv_missing missing,
                               iv_missing missingEnd,
                               iv_missingFiniteValues missingFiniteValues,
                               iv_missingFiniteValues missingFiniteValuesEnd);
    CategoricalSamplerIterator(const CategoricalSamplerIterator& iterator);
    CategoricalSamplerIterator& operator++();
    CategoricalSamplerIterator operator++(int);
    bool operator==(const CategoricalSamplerIterator& rhs) const;
    bool operator!=(const CategoricalSamplerIterator& rhs) const;
    RetValue operator*() const;
    double luSampler(STK::Real lower, STK::Real alpha) const;
    double lrbSampler(STK::Real lower, STK::Real upper) const;
    void findNonEmpty();

  private:
    const STK::Array2D<int>* p_param_;
    const STK::CArrayVector<int>* p_zi_;
    iv_missing iv_missing_;
    iv_missing iv_missingEnd_;
    iv_missingFiniteValues iv_missingFiniteValues_;
    iv_missingFiniteValues iv_missingFiniteValuesEnd_;
    /** Index of currently traversed vector in AugmentedData */
    int currVec_;
    /** Data ranges to compute modalities */
    const std::vector<std::pair<int, int> >& dataRange_;
};

} // namespace mixt

#endif /* MIXT_GAUSSIANSAMPLERITERATOR_H */
