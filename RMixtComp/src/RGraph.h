/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 23, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef RGRAPH_H
#define RGRAPH_H

#include <iostream>
#include <list>
#include <string>
#include "Rcpp.h"
#include <IO/NamedAlgebra.h>
#include <LinAlg/mixt_LinAlg.h>

#include "RTranslate.h"

namespace mixt {

class RGraph {
public:
	RGraph() {};

	RGraph(const Rcpp::List l);

	void set(const Rcpp::List s);

	Rcpp::List getL() const {return l_;}

	void getSubGraph(const std::vector<std::string>& path, RGraph& j) const;

	/**
	 * Add the payload, and create the complete path to it if it does not exist yet.
	 */
	template<typename Type>
	void add_payload(const std::vector<std::string>& path, const std::string& name, const Type& p) {
		add_payload(path, 0, l_, name, p);
	}

	/**
	 * Get the payload and write it in the destination argument.
	 */
	template<typename Type>
	void get_payload(const std::vector<std::string>& path, const std::string& name, Type& p) const {
		Rcpp::List l;
		go_to(path, l);

		if (!Rf_isNull(l[name])) {
			throw(name + " object does not exist.");
		}
		RTranslate(l[name], p);
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
	void go_to(const std::vector<std::string>& path, Rcpp::List l) const;

	void go_to(const std::vector<std::string>& path, Index currDepth, const Rcpp::List currLevel, Rcpp::List) const;

	template<typename Type>
	void add_payload(const std::vector<std::string>& path, Index currDepth, Rcpp::List currLevel, const std::string& name, const Type& p) {
		if (currDepth == path.size()) { // currLevel is the right element in path, add the payload
		  currLevel[name] = p;
		} else {
			Rcpp::List nextLevel = currLevel[path[currDepth]];

			if (!Rf_isNull(nextLevel)) { // if next level does not exist, create it
			  currLevel[path[currDepth]] = Rcpp::List();
			} else if (TYPEOF(nextLevel) != VECSXP) { // if it already exists but is not a json object, throw an exception
				std::string askedPath;
				for (Index i = 0; i < currDepth + 1; ++i) {
					askedPath + "/" + path[i];
				}
				throw(askedPath + " already exists and is not a json object.");
			}

			add_payload(path, currDepth + 1, nextLevel, name, p);
		}
	}

	Rcpp::List l_;
};

}

#endif
