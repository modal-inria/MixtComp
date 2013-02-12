#include <iostream>
#include "src/framework/facade/MixtCompFacade.h"
using namespace std;

int main(){
  FrameworkInfo myinfo;
  //set framework information
  myinfo.algorithm = semgibbs_;
  myinfo.mixturelawlist_ = {multinomial_};
  myinfo.iterations_ = 200l;
  myinfo.strategy_ = iterations_;
  //create object of framework facade
  MixtCompFacade myfacade(myinfo);
  //instantiate framework
  myfacade.instantiateFramework();
  //run the facade
  myfacade.run();

  return 0;
}
