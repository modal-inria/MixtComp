#include <iostream>
#include "framework/facade/MixtCompFacade.h"
using namespace std;

int main(){
  FrameworkInfo myinfo;
  //set framework information
  myinfo.algorithm_= semgibbs_;
  myinfo.mixturelawlist_ = {rank_};
  myinfo.nbIterations_ = 100;
  myinfo.burnin_ = 20;
  myinfo.nbtry_ = 1;
  myinfo.nbCluster_ = 2;
  myinfo.strategy_ = iterations_;
  myinfo.filesep_ = ' ';
  myinfo.datafilename_ = "./data/words_rank.txt";
  myinfo.modalitiesfilename_ = "./data/words_modality.txt";

  //create object of framework facade
  MixtCompFacade myfacade(myinfo);
  //instantiate framework
  if(!myfacade.instantiateFramework())
  {
    std::cerr<<"\n\nCannot instantiate framework.\n";
    exit(0);
  }

  //run the facade
  myfacade.run();
  return 0;
}
