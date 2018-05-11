/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Nov 19, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_PARAMSETTERR_H
#define MIXT_PARAMSETTERR_H

#include "LinAlg/mixt_LinAlg.h"
#include "Mixture/Rank/mixt_RankVal.h"
#include "Rcpp.h"

namespace mixt {

class ParamSetterR {
public:
	ParamSetterR(const Rcpp::List param);

	~ParamSetterR();

	void getParam(const std::string& idName, const std::string& paramName, Vector<Real>& param, std::string& paramStr) const;

	void getParam(const std::string& idName, const std::string& paramName, Vector<RankVal>& param, std::string& paramStr) const;
private:
	const Rcpp::List param_;
};

} // namespace mixt

#endif // MIXT_PARAMSETTERR_H
