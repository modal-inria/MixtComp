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


