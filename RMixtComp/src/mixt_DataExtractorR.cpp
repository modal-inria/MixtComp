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

#include "mixt_DataExtractorR.h"
#include "MixtComp/src/Mixture/StkppMixturesBridges/mixt_Clust_Traits.h"
#include "stkpp/include/Clustering.h"
#include "MixtComp/src/Mixture/StkppMixturesBridges/mixt_MixtureBridge.h"
#include "MixtComp/src/Mixture/StkppMixturesBridges/mixt_CategoricalBridges.h"
#include "MixtComp/src/Mixture/StkppMixturesBridges/mixt_GaussianBridges.h"

namespace mixt
{

DataExtractorR::DataExtractorR(const MixtureComposer* composer, const DataHandlerR* handler):
    composer_(composer),
    handler_(handler)
{}

DataExtractorR::~DataExtractorR() {};

Rcpp::List DataExtractorR::extractVal() const
{
  std::map<std::string, Rcpp::List> mapExportData;
  for (std::vector<IMixture*>::const_iterator it = composer_->v_mixtures().begin();
       it != composer_->v_mixtures().end();
       ++it)
  {
    std::string idModelStr = handler_->info().at((*it)->idName());
    STK::Clust::Mixture idModel = STK::Clust::stringToMixture(idModelStr);
#ifdef MC_DEBUG
    std::cout << "DataExtractorR::extractVal, " << (*it)->idName() << std::endl;
#endif
    switch (idModel)
    {
      case STK::Clust::Gaussian_sjk_:
      {
        GaussianBridge_sjk_m* const p_bridge = dynamic_cast<GaussianBridge_sjk_m* const>(*it);

        const STK::Array2D<STK::Real>* data = &p_bridge->getData()->data_;
        STK::Array2D<int> posMissing;
        STK::Array2D<STK::Real> statMissing;

        p_bridge->getDataStat()->exportVals(posMissing, statMissing);

        Rcpp::NumericMatrix dataR       (data->sizeRows()      , data->sizeCols()      );
        Rcpp::IntegerMatrix posMissingR (posMissing.sizeRows() , posMissing.sizeCols() );
        Rcpp::NumericMatrix statMissingR(statMissing.sizeRows(), statMissing.sizeCols());

        for (int i = 0; i < data->sizeRows(); ++i)
          for (int j = 0; j < data->sizeCols(); ++j)
            dataR(i, j) = data->elt(i, j);

        for (int i = 0; i < posMissing.sizeRows(); ++i)
        {
          dataR(posMissing(i, 0), posMissing(i, 1)) = statMissing(i, 0);  // imputation by expected value estimated in the Gibbs sampler
          for (int j = 0; j < posMissing.sizeCols(); ++j)
          {
            posMissingR(i, j) = posMissing(i, j);
          }
          for (int j = 0; j < statMissing.sizeCols(); ++j)
          {
            statMissingR(i, j) = statMissing(i, j); // saving statistics of estimator for further analysis
          }
        }

        mapExportData[(*it)->idName()] = Rcpp::List::create(Rcpp::Named("data") = dataR,
                                                            Rcpp::Named("posMissing") = posMissingR,
                                                            Rcpp::Named("statMissing") = statMissingR);
      }
      break;

      case STK::Clust::Categorical_pjk_:
      {
        CategoricalBridge_pjk_m* const p_bridge = dynamic_cast <CategoricalBridge_pjk_m* const>(*it);

        const STK::Array2D<int>* data = &p_bridge->getData()->data_;
        STK::Array2D<int> posMissing;
        STK::Array2D<STK::Real> statMissing;

        p_bridge->getDataStat()->exportVals(posMissing, statMissing);

        Rcpp::IntegerMatrix dataR       (data->sizeRows()      , data->sizeCols()      );
        Rcpp::IntegerMatrix posMissingR (posMissing.sizeRows() , posMissing.sizeCols() );
        Rcpp::NumericMatrix statMissingR(statMissing.sizeRows(), statMissing.sizeCols());

        for (int i = 0; i < data->sizeRows(); ++i)
          for (int j = 0; j < data->sizeCols(); ++j)
            dataR(i, j) = data->elt(i, j);

        for (int i = 0; i < posMissing.sizeRows(); ++i)
        {
          int pos;
          statMissing.col(i).maxElt(pos);
          dataR(posMissing(i, 0), posMissing(i, 1)) = pos;  // imputation by the mode
          for (int j = 0; j < posMissing.sizeCols(); ++j)
          {
            posMissingR(i, j) = posMissing(i, j);
          }
          for (int j = 0; j < statMissing.sizeCols(); ++j)
          {
            statMissingR(i, j) = statMissing(i, j); // saving statistics of estimator for further analysis
          }
        }

        mapExportData[(*it)->idName()] = Rcpp::List::create(Rcpp::Named("data") = dataR,
                                                            Rcpp::Named("posMissing") = posMissingR,
                                                            Rcpp::Named("statMissing") = statMissingR);

      }
      break;
    }
  }
  return Rcpp::wrap(mapExportData);
}

} // namespace mixt
