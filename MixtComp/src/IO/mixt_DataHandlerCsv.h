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
 *  Created on: November 14, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_DATAHANDLERCSV_H_
#define MIXT_DATAHANDLERCSV_H_

#include <vector>
#include <map>
#include "boost/regex.hpp"

#include "Arrays/include/STK_Array2D.h"
#include "../Data/mixt_AugmentedData.h"
#include "../Various/mixt_Def.h"
#include "../Various/mixt_IO.h"

#include "DManager/include/STK_ReadWriteCsv.h"

namespace mixt
{

class DataHandlerCsv
{
  public:
    /** map: idData -> model */
    typedef std::map<std::string, std::string> InfoMap;
    /** map: id -> vector of indices in rList_ */
    typedef std::map<std::string, std::vector<int> > DataMap;

    /** default constructor */
    DataHandlerCsv(std::string fileData,
                   std::string fileDesc);
    /** destructor */
    virtual ~DataHandlerCsv();

    /** @return the number of samples (the number of rows of the data) */
    int nbSample() const { return nbSamples_;}
        /** @return the number of variables (the number of columns of the data) */
    int nbVariable() const {return nbVariables_;};

    /** read a data file and its companion description file,
      and fill the infoMap_ (id -> model) and dataMap_ (id -> vector of positions)
      members */
    bool listData();
    
    /** @return the map with the idDatas and idModel of the models */
    InfoMap const& info() const{return info_;};
    bool addInfo(std::string const& idData, std::string const& idModel);
    void writeInfo(std::ostream& os) const;
    /** write information on the localization of data in the rList */
    void writeDataMap() const;

    template<typename Type>
    void getData(std::string const& idData,
                 AugmentedData<STK::Array2D<Type> >& augData,
                 int& nbVariable) const;
  private:
    /** name of the data file */
    std::string dataName_;
    /** name of the descriptor file */
    std::string descName_;

    /** objects to parse the csv files */
    STK::ReadWriteCsv dataContent_;
    STK::ReadWriteCsv descContent_;

    int nbSamples_;
    int nbVariables_;
    /** Store the informations  of the mixtures in the form (idData, idModel) with
     * - idData: an arbitrary idData for a model
     * - model: a string which can be converted in an existing model
     * @sa stringToMixture */
    InfoMap info_;
    /** map: id -> vector of positions in rList_, as typedef-ed above */
    DataMap dataMap_;
};

template<typename Type>
void DataHandlerCsv::getData(std::string const& idData,
                             AugmentedData<STK::Array2D<Type> >& augData,
                             int& nbVariable) const
{
}

} /* namespace mixt */
#endif /* MIXT_DATAHANDLERCSV_H_ */
