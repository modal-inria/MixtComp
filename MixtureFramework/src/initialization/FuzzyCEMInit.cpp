#include "FuzzyCEMInit.h"

FuzzyCEMInit::FuzzyCEMInit()
{
  // TODO Auto-generated constructor stub

}

FuzzyCEMInit::~FuzzyCEMInit()
{
  // TODO Auto-generated destructor stub
}

void FuzzyCEMInit::run(IModel*& p_model){
  p_model->fuzzycemInit();
}
