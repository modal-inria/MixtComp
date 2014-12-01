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
 *  Created on: Nov 14, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "../mixt_MixtComp.h"

#include "../IO/mixt_DataHandlerCsv.h"
#include "../IO/mixt_DataExtractorCsv.h"
#include "../IO/mixt_ParamExtractorCsv.h"

int main()
{
  STK::Real confidenceLevel = 0.95;
  int nbClusters = 3;

  // create the data handler
  mixt::DataHandlerCsv handler("data/gaussian.data.csv",
                               "data/gaussian.desc.csv");
  handler.listData();
  handler.writeInfo(std::cout);
  handler.writeDataMap();

  // create the data extractor
  mixt::DataExtractorCsv dataExtractor;

  // create the parameters extractor
  mixt::ParamExtractorCsv paramExtractor;

  // create the mixture manager
  mixt::MixtureManager<mixt::DataHandlerCsv,
                       mixt::DataExtractorCsv,
                       mixt::ParamExtractorCsv> manager(&handler,
                                                        &dataExtractor,
                                                        &paramExtractor,
                                                        confidenceLevel);

  // prepare the composer
  mixt::MixtureComposer composer(handler.nbSample(),
                                 handler.nbVariable(),
                                 nbClusters);

  manager.createMixtures(&composer,
                         nbClusters);

  // create the appropriate strategy and transmit the parameters
  mixt::SemStrategy strategy(&composer,
                             3, // number of trials of the complete chain
                             114, // number of burn-in iterations
                             507, // number of iterations
                             123, // number of iterations for Gibbs sampler burn-in
                             512, // number of iterations for Gibbs sampler
                             10); // number of sampling attempts for lowly populated classes

  // run the strategy
  strategy.run();
}
