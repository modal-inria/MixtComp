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
 *  Created on: 19 feb. 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_GAUSSIANSAMPLERITERATOR_H
#define MIXT_GAUSSIANSAMPLERITERATOR_H

#include <vector>
#include "Arrays/include/STK_Array2D.h"
#include "Arrays/include/STK_CArrayVector.h"

namespace mixt
{

class GaussianSamplerIterator
{

  typedef std::pair<int, int> pos;
  typedef typename std::vector<          pos                                    >::const_iterator iv_missing;
  typedef typename std::vector<std::pair<pos, std::pair<STK::Real, STK::Real> > >::const_iterator iv_missingIntervals;
  typedef typename std::vector<std::pair<pos,           STK::Real             > >::const_iterator iv_missingLUIntervals;
  typedef typename std::vector<std::pair<pos,           STK::Real             > >::const_iterator iv_missingRUIntervals;
  typedef typename std::pair<pos, STK::Real> RetValue;

  public:
    GaussianSamplerIterator(const STK::Array2D<STK::Real>* p_param,
                            const STK::CArrayVector<int>* p_zi,
                            iv_missing missing,
                            iv_missing missingEnd,
                            iv_missingIntervals missingIntervals,
                            iv_missingIntervals missingIntervalsEnd,
                            iv_missingLUIntervals missingLUIntervals,
                            iv_missingLUIntervals missingLUIntervalsEnd,
                            iv_missingRUIntervals missingRUIntervals,
                            iv_missingRUIntervals missingRUIntervalsEnd);
    GaussianSamplerIterator(const GaussianSamplerIterator& iterator);
    GaussianSamplerIterator& operator++();
    GaussianSamplerIterator operator++(int);
    bool operator==(const GaussianSamplerIterator& rhs) const;
    bool operator!=(const GaussianSamplerIterator& rhs) const;
    RetValue operator*() const;
    double lbSampler(STK::Real lower) const;
    double lrbSampler(STK::Real lower, STK::Real upper) const;
    void findNonEmpty();

  private:
    const STK::Array2D<STK::Real>* p_param_;
    const STK::CArrayVector<int>* p_zi_;
    iv_missing iv_missing_;
    iv_missing iv_missingEnd_;
    iv_missingIntervals iv_missingIntervals_;
    iv_missingIntervals iv_missingIntervalsEnd_;
    iv_missingLUIntervals iv_missingLUIntervals_;
    iv_missingLUIntervals iv_missingLUIntervalsEnd_;
    iv_missingRUIntervals iv_missingRUIntervals_;
    iv_missingRUIntervals iv_missingRUIntervalsEnd_;
    /** Index of currently traversed vector in AugmentedData */
    int currVec_;
};

} // namespace mixt

#endif /* MIXT_GAUSSIANSAMPLERITERATOR_H */
