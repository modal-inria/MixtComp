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

namespace mixt
{

DataExtractorR::DataExtractorR()
{}

DataExtractorR::~DataExtractorR()
{}

void DataExtractorR::exportVals(std::string idName,
                                const STK::Array2D<STK::Real>* p_data,
                                const STK::Array2D<int>* p_posMissing,
                                const STK::Array2D<STK::Real>* p_statMissing)
{
  Rcpp::NumericMatrix dataR       (p_data->sizeRows()       , p_data->sizeCols()       );
  Rcpp::IntegerMatrix posMissingR (p_posMissing->sizeRows() , p_posMissing->sizeCols() );
  Rcpp::NumericMatrix statMissingR(p_statMissing->sizeRows(), p_statMissing->sizeCols());

  for (int i = 0; i < p_data->sizeRows(); ++i)
    for (int j = 0; j < p_data->sizeCols(); ++j)
      dataR(i, j) = p_data->elt(i, j);

  for (int i = 0; i < p_posMissing->sizeRows(); ++i)
  {
    dataR(p_posMissing->elt(i, 0), p_posMissing->elt(i, 1)) = p_statMissing->elt(i, 0);  // imputation by expected value estimated in the Gibbs sampler
    for (int j = 0; j < p_posMissing->sizeCols(); ++j)
    {
      posMissingR(i, j) = p_posMissing->elt(i, j) + 1; // R matrices cols and rows start at 1
    }
    for (int j = 0; j < p_statMissing->sizeCols(); ++j)
    {
      statMissingR(i, j) = p_statMissing->elt(i, j); // saving statistics of estimator for further analysis
    }
  }

  data_[idName] = Rcpp::List::create(Rcpp::Named("data") = dataR,
                                     Rcpp::Named("posMissing") = posMissingR,
                                     Rcpp::Named("statMissing") = statMissingR);

#ifdef MC_DEBUG
  std::cout << "DataExtractorR::exportVals (gaussian), data_.size():  " << data_.size() << std::endl;
#endif
}

void DataExtractorR::exportVals(std::string idName,
                                const STK::Array2D<int>* p_data,
                                const STK::Array2D<int>* p_posMissing,
                                const STK::Array2D<STK::Real>* p_statMissing)
{
  Rcpp::IntegerMatrix dataR       (p_data->sizeRows()      , p_data->sizeCols()      );
  Rcpp::IntegerMatrix posMissingR (p_posMissing->sizeRows() , p_posMissing->sizeCols() );
  Rcpp::NumericMatrix statMissingR(p_statMissing->sizeRows(), p_statMissing->sizeCols());

  for (int i = 0; i < p_data->sizeRows(); ++i)
    for (int j = 0; j < p_data->sizeCols(); ++j)
      dataR(i, j) = p_data->elt(i, j);

#ifdef MC_DEBUG
    std::cout << "DataExtractorR::extractVal, STK::Clust::Categorical_pjk_, posMissing.sizeRows() = " << p_posMissing->sizeRows() << std::endl;
#endif

  for (int i = 0; i < p_posMissing->sizeRows(); ++i)
  {
#ifdef MC_DEBUG
    std::cout << "DataExtractorR::extractVal, STK::Clust::Categorical_pjk_, i = " << i << std::endl;
#endif
    int pos;
    p_statMissing->row(i).maxElt(pos);
    dataR(p_posMissing->elt(i, 0), p_posMissing->elt(i, 1)) = pos;  // imputation by the mode
    for (int j = 0; j < p_posMissing->sizeCols(); ++j)
    {
      posMissingR(i, j) = p_posMissing->elt(i, j) + 1; // R matrices cols and rows start at 1
    }
    for (int j = 0; j < p_statMissing->sizeCols(); ++j)
    {
      statMissingR(i, j) = p_statMissing->elt(i, j); // saving statistics of estimator for further analysis
    }
  }

  data_[idName] = Rcpp::List::create(Rcpp::Named("data") = dataR,
                                     Rcpp::Named("posMissing") = posMissingR,
                                     Rcpp::Named("statMissing") = statMissingR);
#ifdef MC_DEBUG
  std::cout << "DataExtractorR::exportVals (categorical), data_.size():  " << data_.size() << std::endl;
#endif
}

Rcpp::List DataExtractorR::rcppReturnVals() const
{
  return Rcpp::wrap(data_);
}

} // namespace mixt
