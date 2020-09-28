/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>
 **/

/*
 *  Project:    MixtComp
 *  Created on: September 28, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <boost/algorithm/string.hpp>
#include <Mixture/Rank/RankISRParser.h>

#include "../../IO/MisValParser.h"
#include "../../IO/IO.h"

namespace mixt {

std::string parseRankStr(const std::vector<std::string>& vecStr, int minMod, int& nbPos, Vector<RankISRIndividual>& vecInd) {
	typedef std::pair<MisType, std::vector<int> > MisVal;

	int nbInd = vecStr.size();
	std::string warnLog;

	vecInd.resize(nbInd);

	MisValParser<int> mvp(-minMod);

	std::vector<std::string> strs;
	boost::split(strs, vecStr[0], // number of modalities is deduced from the first individual and a warning will be triggered if that is not the case
	boost::is_any_of(rankPosSep));
	nbPos = strs.size();

	for (int i = 0; i < nbInd; ++i) {
		Vector<int> o(nbPos);
		Vector<MisVal> obsData(nbPos);

		vecInd(i).setNbPos(nbPos);

		boost::split(strs, vecStr[i], boost::is_any_of(rankPosSep));

		if (strs.size() != nbPos) {
			std::stringstream sstm;
			sstm << "Individual i: " << i << " has " << strs.size() << " modalities, which is less than the previous individuals. They all had " << nbPos
					<< " modalities. Please check that all individuals in this Rank variable all have the same number of modalities." << std::endl;
			warnLog += sstm.str();
			return warnLog;
		}

		for (int p = 0; p < nbPos; ++p) {
			int min = 0;
			int max = 0;

			bool isValid = mvp.parseStr(strs[p], o(p), obsData(p));

			switch (obsData(p).first) {
			case present_: {
				min = o(p);
				max = o(p);
			}
				break;

			case missing_: {
				min = 0;
				max = nbPos - 1;
			}
				break;

			case missingFiniteValues_: {
				min = *(std::min_element(obsData(p).second.begin(), obsData(p).second.end()));
				max = *(std::max_element(obsData(p).second.begin(), obsData(p).second.end()));
			}
				break;

			default: {
				return "Data in Rank must be of types: present, missing, or missing finite values. " + strs[p] + "is not valid.";
			}
				break;
			}

			if (min < 0) {
				std::stringstream sstm;
				sstm << "Individual i: " << i << " in (0-based) position " << p << " has minimum value " << min + minModality
						<< " which is forbidden. The lowest acceptable value is " << minModality << std::endl;
				warnLog += sstm.str();
			}

			if (nbPos - 1 < max) {
				std::stringstream sstm;
				sstm << "Individual i: " << i << " in (0-based) position " << p << " has maximum value " << max + minModality
						<< " which is forbidden. The lowest acceptable value is " << nbPos - 1 + minModality << std::endl;
				warnLog += sstm.str();
			}

			if (!isValid) {
				std::stringstream sstm;
				sstm << "Individual i: " << i << " present an error. " << strs[p] << " is not recognized as a valid format for a Rank position." << std::endl;
				warnLog += sstm.str();
			}
		}

		if (warnLog.size() > 0) { // in the absence of return for an erroneous data, following instructions will crash
			return warnLog;
		}

		vecInd(i).setO(o);
		vecInd(i).setObsData(obsData);

		if (vecInd(i).enumCompleted().size() == 0) {
#ifdef MC_DEBUG
			std::cout << "o: " << itString(o) << std::endl;
			std::cout << "obsData: " << std::endl;
			for (int p = 0; p < nbPos; ++p) {
				std::cout << "p: " << p << ", MisType: " << obsData(p).first << ", values: " << itString(obsData(p).second) << std::endl;
			}
#endif

			std::stringstream sstm;
			sstm << "Individual " << i << " which data is " << vecStr[i] << " is invalid. Please check if there are no repeated " << "observed modalities for example."
					<< std::endl;
			warnLog += sstm.str();
		}
	}

	return warnLog;
}
} // namespace mixt
