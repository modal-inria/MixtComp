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
	boost::regex vectorRE(
			R"(Vector<([a-zA-Z]+)>\(([0-9]+)\))");
	boost::regex matrixRE(R"(Matrix<([a-zA-Z]+)>\(([0-9]+),([0-9]+)\))");

	for (nlohmann::json::const_iterator it = json.begin(); it != json.end();
			++it) { // if json is not a NamedAlgebra, simply loop over all elements

		if ((*it).is_object()) { // if object is a json, check wether the subobject is a NamedAlgebra or not
			if ((*it).find("dtype") != (*it).end()) { // if there is a dtype string, that means that the current json represents a NamedAlgebra object, which requires a particular parsing
				std::string dtype = (*it)["dtype"].get<std::string>();
				boost::cmatch what;
				if (boost::regex_match(dtype.c_str(), what, vectorRE)) { // pattern match a vector
					std::string dataType = what[1];
					Index ncol = toIndex(what[2].str());
					std::vector<std::string> colNames = (*it)["colNames"].get<std::vector<std::string>>();

					if (dataType == "Real") { // Vector<Real>
						std::vector<Real> dataRaw = (*it)["data"].get<std::vector<Real>>();
						if (dataRaw.size() != ncol) throw("Error in Vector<Real>, number of elements in data not the same as expected in dtype field");
						Vector<Real> data(ncol);
						for (Index i = 0; i < ncol; ++i) {
							data(i) = dataRaw[i];
						}

						NamedVector<Real> nv = {colNames, data};
						res.add_payload(it.key(), nv);
					} else {
						throw(std::string(dataType) + " dtype not supported yet.");
					}
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
