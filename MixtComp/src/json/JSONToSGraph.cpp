/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Aug 3, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <iostream>
#include <boost/regex.hpp>

#include <LinAlg/mixt_LinAlg.h>
#include "JSONToSGraph.h"

namespace mixt {

/**
 * https://www.boost.org/doc/libs/1_67_0/libs/regex/doc/html/boost_regex/ref/regex_match.html
 */
SGraph JSONToSGraph(const nlohmann::json& json) {
	SGraph res;

	for (nlohmann::json::const_iterator it = json.begin(); it != json.end(); ++it) { // if json is not a NamedAlgebra, simply loop over all elements

		if ((*it).is_object()) { // if object is a json, check wether the subobject is a NamedAlgebra or not
			if ((*it).find("ctype") != (*it).end()) { // if there is a ctype string, that means that the current json represents a NamedAlgebra object, which requires a particular parsing
				std::string ctype = (*it)["ctype"].get<std::string>();
				std::string dtype = (*it)["dtype"].get<std::string>();
				Index nrow = (*it)["nrow"].get<Index>();

				if (ctype == "Vector") { // pattern match a Vector
					std::vector<std::string> rowNames = (*it)["rowNames"].get<std::vector<std::string>>();

					if (dtype == "Real") { // Vector<Real>
						std::vector<Real> dataRaw = (*it)["data"].get<std::vector<Real>>();

						Vector<Real> data(nrow);
						for (Index i = 0; i < nrow; ++i) {
							data(i) = dataRaw[i];
						}

						NamedVector<Real> nv = { rowNames, data };
						res.add_payload(it.key(), nv);
					} else {
						throw(it.key() + ": " + std::string(dtype) + " dtype not supported yet.");
					}

				} else if (ctype == "Matrix") { // pattern match a Matrix
					Index ncol = (*it)["nrow"].get<Index>();

					std::vector<std::string> rowNames = (*it)["rowNames"].get<std::vector<std::string>>();
					std::vector<std::string> colNames = (*it)["colNames"].get<std::vector<std::string>>();

					if (dtype == "Real") { // Matrix<Real>
						std::vector<std::vector<Real>> dataRaw = (*it)["data"].get<std::vector<std::vector<Real>>>();

						for (std::vector<std::vector<Real>>::const_iterator it2 = dataRaw.begin(), it2End = dataRaw.end(); it2 != it2End; ++it2) {
							if ((*it2).size() != ncol)
								throw(it.key() + ": number of columns in data not the same as expected in dtype field");
						}

						Matrix<Real> data(nrow, ncol);
						for (Index i = 0; i < ncol; ++i) {
							for (Index j = 0; j < ncol; ++j) {
								data(i, j) = dataRaw[i][j];
							}
						}

						NamedMatrix<Real> nm = { rowNames, colNames, data };
						res.add_payload(it.key(), nm);
					} else {
						throw(it.key() + ": " + dtype + " dtype not supported yet.");
					}

				} else { // get out of my lawn
					throw(it.key() + ": " + ctype + " ctype not supported yet.");
				}
			} else { // recursive call
				SGraph converted = JSONToSGraph(*it);
				res.add_child(it.key(), converted);
			}

		} else if ((*it).is_number_unsigned()) { // Index
			Index val = (*it).get<Index>();
			res.add_payload(it.key(), val);
		} else if ((*it).is_number_float()) { // Real
			Real val = (*it).get<Real>();
			res.add_payload(it.key(), val);
		} else if ((*it).is_string()) { // std::string
			std::string val = (*it).get<std::string>();
			res.add_payload(it.key(), val);
		} else {
			throw(std::string((*it).type_name()) + " not supported yet.");
		}

	}

	return res;
}

}
