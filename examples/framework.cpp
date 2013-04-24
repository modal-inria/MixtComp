#include <iostream>
#include "framework/facade/MixtCompFacade.h"
using namespace std;

int main(){
  FrameworkInfo myinfo;
  //set framework information
  myinfo.algorithm_= semgibbs_;
  myinfo.mixturelawlist_ = {rank_};
  myinfo.nbIterations_ = 500;
  myinfo.burnin_ = 50;
  myinfo.nbtry_ = 1;
  myinfo.nbCluster_ = 2;
  myinfo.nbSample_ = 1000;
  myinfo.strategy_ = iterations_;
  //create object of framework facade
  MixtCompFacade myfacade(myinfo);
  //instantiate framework
  myfacade.instantiateFramework();
  //run the facade
  myfacade.run();
  return 0;
}
