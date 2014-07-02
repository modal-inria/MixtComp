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
 *  Created on: July 1, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_CATEGORICALDATASTAT_H
#define MIXT_CATEGORICALDATASTAT_H

namespace mixt
{

class CategoricalDataStat
{
  public:
    CategoricalDataStat(const AugmentedData<STK::Array2D<int> >* pm_augDataij);
    ~CategoricalDataStat();
    void sampleVals();

  private:
    // number of iterations used to compute the statistics
    int nbIter;
    const AugmentedData<STK::Array2D<int> >* pm_augDataij_;

};

} // namespace mixt

#endif // MIXT_CATEGORICALDATASTAT_H
