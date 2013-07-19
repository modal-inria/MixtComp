#include "RandomInit.h"

RandomInit::RandomInit()
{
  // TODO Auto-generated constructor stub

}

RandomInit::~RandomInit()
{
  // TODO Auto-generated destructor stub
}

void RandomInit::run(IModel*& p_model){
  p_model->randomInit();
}
