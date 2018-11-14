/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 25, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 **/

#ifndef MIXT_EXPORT_H_
#define MIXT_EXPORT_H_

#include <iostream>
#include <sstream>
#include <fstream>

namespace mixt {

template<typename Type>
Type str2type(const std::string& s) {
	std::istringstream i(s);
	Type x;
	i >> x;
	return x;
}

template<typename Type>
std::string type2str(const Type& value) {
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

template<typename Type>
void writeDataCsv(std::string fileName, const Type& data) {
#ifdef MC_VERBOSE
	std::cout << "writing: " << fileName << std::endl;
#endif
	std::ofstream stream;
	stream.open(fileName.c_str());
	for (int i = 0; i < data.rows(); ++i) {
		for (int j = 0; j < data.cols(); ++j) {
			stream << data(i, j);
			if (j < data.cols())
				stream << ";";
		}
		stream << std::endl;
	}
	stream.close();
}

/**
 * Sequentially output elements of iterable object to a string
 *
 * @param obj object to iterate on
 */
template<typename Type>
std::string itString(const Type& obj) {
	if (obj.size() == 0) {
		return std::string();
	}

	std::stringstream sstm;
	typename Type::const_iterator it = obj.begin();
	typename Type::const_iterator itEnd = obj.end();

	sstm << *it;
	++it;

	for (; it != itEnd; ++it) {
		sstm << " " << *it;
	}

	return sstm.str();
}

} // namespace mixt

#endif /* MIXT_EXPORT_H_ */
