#include <fstream>
#include <iomanip>
#include <iostream>

#include <Run/Learn.h>
#include <Run/Predict.h>
#include <Various/mixt_Constants.h>
#include "PyGraph.hpp"

using namespace mixt;

int main(int argc, char* argv[]) {
  try {
    std::cout << "PyMixtComp" << std::endl;

    if (argc < 4) {
      std::cout << "PyMixtComp should be called with 4 parameters (paths to "
                   "algo, data, desc, resLearn) in learn and 5 parameters "
                   "(paths to algo, data, desc, resLearn, resPredict) in "
                   "predict. It has been called with "
                << argc - 1 << " parameters." << std::endl;
      return 0;
    }

    std::string warnLog;
    std::string algoFile = argv[1];
    std::string dataFile = argv[2];
    std::string descFile = argv[3];
    std::string resLearnFile = argv[4];

    std::ifstream algoStream(algoFile);
    std::ifstream dataStream(dataFile);
    std::ifstream descStream(descFile);

    if (algoStream.good() == false || dataStream.good() == false ||
        descStream.good() == false) {
      std::cout << "Check that algo: " << algoFile << ", data: " << dataFile
                << ", and desc: " << descFile << " paths are correct"
                << std::endl;
    } else {
      nlohmann::json algoJSON;
      algoStream >> algoJSON;
      JSONGraph algoG(algoJSON);

      nlohmann::json dataJSON;
      dataStream >> dataJSON;
      JSONGraph dataG(dataJSON);

      nlohmann::json descJSON;
      descStream >> descJSON;
      JSONGraph descG(descJSON);

      std::string mode = algoG.get_payload<std::string>({}, "mode");

      JSONGraph resG;
      std::string resFile;

      if (mode == "learn") {
        resFile = resLearnFile;
        learn(algoG, dataG, descG, resG);
      } else if (mode == "predict") {
        if (argc != 6) {
          std::cout << "JMixtComp should be called with 5 parameters (paths to "
                       "algo, data, desc, resLearn, resPredict) in predict. It "
                       "has been called with "
                    << argc - 1 << " parameters." << std::endl;
          return 0;
        }

        resFile = argv[5];

        std::ifstream resLearnStream(resLearnFile);
        nlohmann::json resLearnJSON;
        resLearnStream >> resLearnJSON;

        try {
          JSONGraph paramG(resLearnJSON["variable"]["param"]);
          predict(algoG, dataG, descG, paramG, resG);
        } catch (const std::string& s) {
          warnLog += s;
        }

      } else {
        warnLog += "mode :" + mode +
                   " not recognized. Please choose learn or predict." + eol;
      }

      if (warnLog.size() > 0) {
        resG.add_payload({}, "warnLog", warnLog);
      }

      std::ofstream o(resFile);
      o << std::setw(4) << resG.getJ() << std::endl;
    }
  } catch (const std::string& s) {
    std::cout << s << std::endl;
  }

  return 0;
}
