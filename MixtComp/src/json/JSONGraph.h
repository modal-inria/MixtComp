/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/*
 *  Project:    MixtComp
 *  Created on: August 21, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef JSON_JSONGRAPH_H
#define JSON_JSONGRAPH_H

#include <iostream>
#include <list>
#include "json.hpp"
#include <IO/NamedAlgebra.h>
#include <IO/IOFunctions.h>
#include <LinAlg/LinAlg.h>
#include <translateCPPToJSON.h>
#include <translateJSONToCPP.h>

namespace mixt {

class JSONGraph {
public:
	JSONGraph() {};

	JSONGraph(const nlohmann::json& j);

	void set(const std::string& s);

	void set(const nlohmann::json& j);

	std::string get() const;

	const nlohmann::json& getJ() const {return j_;}

	void getSubGraph(const std::vector<std::string>& path, JSONGraph& j) const;

	/**
	 * Add the payload, and create the complete path to it if it does not exist yet.
	 */
	template<typename Type>
	void add_payload(const std::vector<std::string>& path, const std::string& name, const Type& p) {
		add_payload(path, 0, j_, name, p);
	}

	void addSubGraph(const std::vector<std::string>& path, const std::string& name, const JSONGraph& p);

	/**
	 * Get the payload and write it in the destination argument.
	 */
	template<typename Type>
	void get_payload(const std::vector<std::string>& path, const std::string& name, Type& p) const {
		nlohmann::json l;
		go_to(path, l);

		if (l[name].is_null()) {
			std::string cPath;
			completePath(path, name, cPath);
			throw(cPath + " object does not exist.");
		}
		translateJSONToCPP(l[name], p);
	}

	/**
	 * Get the payload as a return value. Useful in a few cases like filling values in a constructor.
	 */
	template<typename Type>
	Type get_payload(const std::vector<std::string>& path, const std::string& name) const {
		Type val;
		get_payload(path, name, val);
		return val;
	}

	bool exist_payload(const std::vector<std::string>& path, const std::string& name) const;

	void name_payload(const std::vector<std::string>& path, std::list<std::string>& l) const;

private:
	void go_to(const std::vector<std::string>& path, nlohmann::json& l) const;

	void go_to(const std::vector<std::string>& path, Index currDepth, const nlohmann::json& currLevel, nlohmann::json&) const;

	template<typename Type>
	void add_payload(const std::vector<std::string>& path, Index currDepth, nlohmann::json& currLevel, const std::string& name, const Type& p) {
		if (currDepth == path.size()) { // currLevel is the right element in path, add the payload
			translateCPPToJSON(p, currLevel[name]);
		} else {
			nlohmann::json& nextLevel = currLevel[path[currDepth]];

			if (nextLevel.is_null()) { // if next level does not exist, create it
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

	void addSubGraph(const std::vector<std::string>& path, Index currDepth, nlohmann::json& currLevel, const std::string& name, const JSONGraph& p);

	nlohmann::json j_;
};

}

#endif
