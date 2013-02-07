#include "CompositeMixtureLaw.h"

CompositeMixtureLaw::CompositeMixtureLaw()
{
  // TODO Auto-generated constructor stub
}


CompositeMixtureLaw::~CompositeMixtureLaw()
{
  for (int i = 0; i < _parameterComponent.size(); ++i) {
    if(_parameterComponent[i]) delete _parameterComponent[i];
    if(_parameterModelType[i]) delete _parameterModelType[i];
  }

}


void CompositeMixtureLaw::getAllPdf(double** tabFik,double* tabProportion)const
{
}


double CompositeMixtureLaw::GetPdf(int sample_num,int cluster_num) const
{
  double pdfval = 1.0;
  for (i = 0; i < v_MixtureComponent_.size(); ++i) {
    pdfval*= v_MixtureComponent_[i]->GetPdf(sample_num,cluster_num);
  }

  return pdfval;
}

void CompositeMixtureLaw::MStep()
{
  for (int i = 0; i < v_MixtureComponent_.size(); ++i) {
    v_MixtureComponent_[i]->Mstep;
  }
}

CompositeMixtureLaw * CompositeMixtureLaw::clone() const
{
  CompositeMixtureLaw * newparam = new CompositeMixtureLaw(this);
  return newparam;
}

int64_t CompositeMixtureLaw::getFreeParameter() const
{
  int64_t freeparam = _parameterComponent[1]->getFreeParameter()+_parameterComponent[0]->getFreeParameter()-(_nbCluster-1);
  return freeparam;
}
