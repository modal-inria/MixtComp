#include <iostream>
#include <string>
#include "stkpp/include/STKpp.h"
#include "include.h"
using namespace std;
int main(int argc,char** argv){
  string filename = argv[1];
  STK::ReadWriteCsv rw(filename, true, STK::String(1,','));
  if (!rw.read())
  {
    cout<<"Cannot read from file\n";
    exit(1);
  }

  int nbsamples = rw.rows().size();
  int nbvariables = rw.sizeCol();
  STK::Array2D<double>  data(nbsamples,nbvariables);
  for (int i = 0; i < nbsamples; ++i) {
    for (int j = 0; j < nbvariables; ++j) {
      data(i,j) = STK::stringToType<double>(rw(i,j));
    }
  }

  //model
  IModel* p_model = new gaussianMixture(data,2);
  //algorithm
  IAlgo* p_algo = new EMAlgo();
  //initialization
  IInit* p_init = new RandomInit();
  //strategy parameters
  SimpleStratParam param;
  param.epsilon_ = .0001;
  param.nbIterations_ = 200;
  param.nbTry_ = 5;
  //strategy
  IStrategy* p_strategy = new SimpleStrategy(param);
  //facade
  facade facade_obj(p_model,p_algo,p_init,p_strategy);

  //run
  facade_obj.run();

  //display results
  dynamic_cast<gaussianMixture*>(p_model)->writeParameters(std::cout);

  return 0;

}



