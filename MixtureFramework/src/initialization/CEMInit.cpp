#include "CEMInit.h"

CEMInit::CEMInit()
{
  // TODO Auto-generated constructor stub

}

CEMInit::~CEMInit()
{
  // TODO Auto-generated destructor stub
}

void CEMInit::run(IModel*& p_model){
  p_model->cemInit();
}
