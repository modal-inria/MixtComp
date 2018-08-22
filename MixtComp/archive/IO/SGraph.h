/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 3, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef SGRAPH_H
#define SGRAPH_H

#include <IO/AlgTypeVisitor.h>
#include <map>
#include <LinAlg/mixt_LinAlg.h>
#include "AlgType.h"


namespace mixt {

/**
 * Graph for data serialization. The main objective of this class is to allow building of the output data directly inside the models. The translation to the IO module is done at the end. This is in stark contrast with the previous
 * implementation, where the translation was done locally. In the previous implementation, the IO module is pervasive, and every mixture is templated with it, which was quite an ugly architecture.
 */
class SGraph {
public:
	/** Generic method to add a basic type. */
	void add_payload(const std::string& name, const AlgType& data);

	void add_child(const std::string& name, const SGraph& child);

	template<typename Type>
	Type get_payload(const std::string& name) const {
		return boost::apply_visitor(AlgTypeVisitor<Type>(), payload_.at(name));
	}

	const SGraph& get_child(const std::string& name) const {
		return children_.at(name);
	}

	bool exist_payload(const std::string& name) const;
	bool exist_child(const std::string& name) const;

	/** Map to loop over all payload elements.*/
	const std::map<std::string, AlgType>& get_payload() const {
		return payload_;
	}

	/** Map to loop over all children.*/
	const std::map<std::string, SGraph>& get_children() const {
		return children_;
	}

	void listElements() const;
private:
	std::string dummyStr_;

	/** Payload of current node.*/
	std::map<std::string, AlgType> payload_;

	/** Links to other nodes. */
	std::map<std::string, SGraph> children_;
};

}

#endif
