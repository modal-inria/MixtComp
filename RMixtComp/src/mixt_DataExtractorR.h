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
 *  Created on: July 7, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_DATAEXTRACTOR_H
#define MIXT_DATAEXTRACTOR_H

#include "MixtComp/src/Composer/mixt_MixtureComposer.h"
#include "mixt_DataHandlerR.h"
#include "MixtComp/src/Mixture/mixt_IMixture.h"

namespace mixt
{

class DataExtractorR
{
  public:
    DataExtractorR(const MixtureComposer* composer, const DataHandlerR* handler);
    ~DataExtractorR();
    void extractVal(Rcpp::List list) const;
  private:
    const MixtureComposer* composer_;
    const DataHandlerR* handler_;
};

} // namespace mixt

#endif // MIXT_DATAEXTRACTOR_H
