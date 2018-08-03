/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 3, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef SGRAPH_H
#define SGRAPH_H

#include <map>
#include "boost/variant.hpp"
#include <LinAlg/mixt_LinAlg.h>

namespace mixt {

/**
 * boost::variant provides the equivalent of algebraic type. The main advantage vs polymorphism is that the syntax is lighter. The serialization graph is only used to carry the data from the depths of the model to
 * the IO module. Therefore using polymorphism would be overkill.
 */
typedef typename boost::variant<Index, Real, std::string> AlgType;

/**
 * Graph for data serialization. The main objective of this class is to allow building of the output data directly inside the models. The translation to the IO module is done at the end. This is in stark contrast with the previous
 * implementation, where the translation was done locally. In the previous implementation, the IO module is pervasive, and every mixture is templated with it, which was quite an ugly architecture.
 */
class SGraph {
public:
	/** Generic method to add a basic type. Should be overloaded for complex types, for example matrices with row names and col names. */
	void add_payload(const std::string& name, const AlgType& data);

	void add_child(const std::string& name, const SGraph& child);

	const AlgType& get_payload(const std::string& name) {
		return payload_[name];
	}
	const SGraph& get_child(const std::string& name) {
		return children_[name];
	}

	/** Map to loop over all payload elements.*/
	const std::map<std::string, AlgType>& get_payload() const {
		return payload_;
	}

	/** Map to loop over all children.*/
	const std::map<std::string, SGraph>& get_children() const {
		return children_;
	}
private:
	/** Payload of current node.*/
	std::map<std::string, AlgType> payload_;

	/** Links to other nodes. */
	std::map<std::string, SGraph> children_;
};

class SGraphPrintVis: public boost::static_visitor<std::string> {
public:
	std::string operator()(Index i) const {
		return std::to_string(i);
	}

	std::string operator()(const std::string& str) const {
		return str;
	}
};

}

#endif
