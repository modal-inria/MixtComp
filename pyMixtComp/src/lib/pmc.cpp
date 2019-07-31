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
 *  Created on: June 12, 2019
 *  Author:    Quentin Grimonprez, Vincent Kubicki
 **/

#include <iostream>
#include <boost/python.hpp>
#include "pmc.h"
#include <Run/Learn.h>
#include <Run/Predict.h>
#include <Various/Constants.h>
#include "PyGraph.h"

using namespace mixt;


boost::python::dict pmc(boost::python::dict algoPy, boost::python::dict dataPy, boost::python::dict descPy, boost::python::dict resLearnPy) {
	PyGraph resPyG;

	try {
		std::string warnLog;

		PyGraph algoPyG(algoPy);
		PyGraph dataPyG(dataPy);
		PyGraph descPyG(descPy);

		PyGraph resLearnPyG(resLearnPy);

		std::string mode = algoPyG.get_payload<std::string>( { }, "mode");

		if (mode == "learn") {
			learn(algoPyG, dataPyG, descPyG, resPyG);
		} else if (mode == "predict") {
			PyGraph resLearnPyG(resLearnPy);

			try {
				PyGraph paramPyG;
				resLearnPyG.getSubGraph( { "variable", "param" }, paramPyG);
				predict(algoPyG, dataPyG, descPyG, paramPyG, resPyG);
			} catch (const std::string& s) {
				warnLog += s;
			}
		} else {
			warnLog += "mode :" + mode + " not recognized. Please choose learn or predict." + eol;
		}

		if (warnLog.size() > 0) {
			resPyG.add_payload( { }, "warnLog", warnLog);
		}
	} catch (const std::string& s) {
		std::cout << s << std::endl;
	}

	return resPyG.getD();
}


