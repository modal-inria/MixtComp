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

  int nbsamples = rw.size();
  int nbvariables = rw.sizeCol();
  STK::Array2D<double>  data(nbsamples,nbvariables);
  for (int i = 0; i < nbsamples; ++i) {
    for (int j = 0; j < nbvariables; ++j) {
      data(i,j) = STK::stringToType<double>(rw(i,j));
    }
  }

  IModel* p_model = gaussianMixture(data,2);
  IAlgo* p_algo = EMAlgo();
  IInit* p_init = RandomInit();
  IStrategy* p_stragey = SimpleStrategy();




}
