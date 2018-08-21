/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 21, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef JSON_JSONGRAPH_H
#define JSON_JSONGRAPH_H

#include "json.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <IO/NamedAlgebra2.h>
#include <LinAlg/mixt_LinAlg.h>

namespace mixt {

template<typename InType, typename OutType>
void translation2(const InType& in, OutType& out) {
	out = in;
}

class JSONGraph {
public:
	void set(const std::string& s);

	std::string get() const;

	/**
	 * Add the payload, and create the complete path to it if it does not exist yet.
	 */
	template<typename Type>
	void add_payload(const std::vector<std::string>& path, const std::string& name, const Type& p) {
		add_payload(path, 0, j_, name, p);
	}

	/**
	 * Get the payload and write it in the destination argument.
	 */
	template<typename Type>
	void get_payload(const std::vector<std::string>& path, const std::string& name, Type& p) const {
		get_payload(path, 0, j_, name, p);
	}

private:
	template<typename Type>
	void add_payload(const std::vector<std::string>& path, Index currDepth, nlohmann::json& currLevel, const std::string& name, const Type& p) {
		std::cout << "currDepth: " << currDepth << std::endl;
		if (currDepth == path.size()) { // currLevel is the right element in path, add the payload
			std::cout << "adding payload" << std::endl;
			// TODO: add translation code for types not supported out of the box by json (there is a mechanism for that...)
			translation2(p, currLevel[name]);
		} else {
			nlohmann::json& nextLevel = currLevel[path[currDepth]];

			if (nextLevel.is_null()) { // if next level does not exist, create it
				std::cout << "is_null" << std::endl;
				nextLevel = nlohmann::json();
			} else if (!nextLevel.is_object()) { // if it already exists but is not a json object, throw an exception
				std::string askedPath;
				for (Index i = 0; i < currDepth + 1; ++i) {
					askedPath + "/" + path[i];
				}
				throw(askedPath + " already exists and is not a json object.");
			}

			add_payload(path, currDepth + 1, nextLevel, name, p);
		}
	}

	template<typename Type>
	void get_payload(const std::vector<std::string>& path, Index currDepth, const nlohmann::json& currLevel, const std::string& name, Type& p) const {
		if (currDepth == path.size()) {
			if (currLevel[name].is_null()) {
				throw(name + " object does not exist.");
			}
			translation2(currLevel[name], p);
		} else {
			const nlohmann::json& nextLevel = currLevel[path[currDepth]];
			if (nextLevel.is_null()) { // if next level does not exist, create it
				std::cout << "is_null" << std::endl;
				std::string askedPath;
				for (Index i = 0; i < currDepth + 1; ++i) {
					askedPath + "/" + path[i];
				}
				throw(askedPath + " path does not exist.");
			}

			get_payload(path, currDepth + 1, nextLevel, name, p);
		}
	}

	nlohmann::json j_;
};

template<>
void translation2(const NamedVector2<Real>& in, nlohmann::json& out);

template<>
void translation2(const nlohmann::json& in, NamedVector2<Real>& out);

template<>
void translation2(const NamedMatrix2<Real>& in, nlohmann::json& out);

template<>
void translation2(const nlohmann::json& in, NamedMatrix2<Real>& out);

}

#endif
