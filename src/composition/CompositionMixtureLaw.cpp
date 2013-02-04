XEMCompositeParameter::XEMCompositeParameter()
{
  // TODO Auto-generated constructor stub
}

XEMCompositeParameter::XEMCompositeParameter(const XEMCompositeParameter * param):XEMParameter(param->getModel(),param->getModelType())
{
  _parameterComponent.resize(2);
  _parameterModelType.resize(2);
  _parameterComponent[0] = (const_cast<XEMCompositeParameter*>(param)->getBinaryParameter())->clone();
  _parameterComponent[1] = ((const_cast<XEMCompositeParameter*>(param)->getGaussianParameter()))->clone();
  _parameterModelType[0] = new XEMModelType(_parameterComponent[0]->getModelType()->_nameModel);
  _parameterComponent[0]->setModelType(_parameterModelType[0]);
  _parameterModelType[1] = new XEMModelType(_parameterComponent[1]->getModelType()->_nameModel);
  _parameterComponent[1]->setModelType(_parameterModelType[1]);

}

XEMCompositeParameter::XEMCompositeParameter(const XEMParameter * igaussian,const XEMParameter * ibinary,
                                             XEMModelType * imodelType):XEMParameter(ibinary->getNbCluster(),
                                                                                     ibinary->getPbDimension()+igaussian->getPbDimension(),
                                                                                     imodelType)
{
  _parameterComponent.resize(2);
  _parameterModelType.resize(2);
  _parameterComponent[0] = (const_cast<XEMParameter*>(ibinary)->getBinaryParameter())->clone();
  _parameterComponent[1] = ((const_cast<XEMParameter*>(igaussian)->getGaussianParameter()))->clone();
  _parameterModelType[0] = new XEMModelType(_parameterComponent[0]->getModelType()->_nameModel);
  _parameterComponent[0]->setModelType(_parameterModelType[0]);
  _parameterModelType[1] = new XEMModelType(_parameterComponent[1]->getModelType()->_nameModel);
  _parameterComponent[1]->setModelType(_parameterModelType[1]);
}

XEMCompositeParameter::XEMCompositeParameter(XEMModel* iModel,XEMModelType* iModelType,int64_t * tabNbModality):XEMParameter(iModel,iModelType)
{
  _parameterComponent.resize(2);
  _parameterModelType.resize(2);
  //Instantiate Binary and Gaussian models
  InstantiateBinaryandGaussianParamter(iModelType,tabNbModality);
}

XEMCompositeParameter::~XEMCompositeParameter()
{
  for (int i = 0; i < _parameterComponent.size(); ++i) {
    if(_parameterComponent[i]) delete _parameterComponent[i];
    if(_parameterModelType[i]) delete _parameterModelType[i];
  }

}

void XEMCompositeParameter::InstantiateBinaryandGaussianParamter(XEMModelType* modeltype,int64_t * tabNbModality)
{
  XEMModelName modelname = modeltype->getModelName();
  switch (modelname) {
    case Heterogeneous_p_E_L_B:
    {
      _parameterModelType[0] = new XEMModelType(Binary_p_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_p_L_B);
      _parameterComponent[0] = new XEMBinaryEParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_p_E_Lk_B:
    {
      _parameterModelType[0] = new XEMModelType(Binary_p_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_p_Lk_B);
      _parameterComponent[0] = new XEMBinaryEParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_p_E_L_Bk:
    {
      _parameterModelType[0] = new XEMModelType(Binary_p_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_p_L_Bk);
      _parameterComponent[0] = new XEMBinaryEParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_p_E_Lk_Bk:
    {
      _parameterModelType[0] = new XEMModelType(Binary_p_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_p_Lk_Bk);
      _parameterComponent[0] = new XEMBinaryEParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_p_Ek_L_B:
    {
      _parameterModelType[0] = new XEMModelType(Binary_p_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_p_L_B);
      _parameterComponent[0] = new XEMBinaryEkParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_p_Ek_Lk_B:
    {
      _parameterModelType[0] = new XEMModelType(Binary_p_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_p_Lk_B);
      _parameterComponent[0] = new XEMBinaryEkParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_p_Ek_L_Bk:
    {
      _parameterModelType[0] = new XEMModelType(Binary_p_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_p_L_Bk);
      _parameterComponent[0] = new XEMBinaryEkParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_p_Ek_Lk_Bk:
    {
      _parameterModelType[0] = new XEMModelType(Binary_p_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_p_Lk_Bk);
      _parameterComponent[0] = new XEMBinaryEkParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_p_Ej_L_B:
    {
      _parameterModelType[0] = new XEMModelType(Binary_p_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_p_L_B);
      _parameterComponent[0] = new XEMBinaryEjParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_p_Ej_Lk_B:
    {
      _parameterModelType[0] = new XEMModelType(Binary_p_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_p_Lk_B);
      _parameterComponent[0] = new XEMBinaryEjParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_p_Ej_L_Bk:
    {
      _parameterModelType[0] = new XEMModelType(Binary_p_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_p_L_Bk);
      _parameterComponent[0] = new XEMBinaryEjParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_p_Ej_Lk_Bk:
    {
      _parameterModelType[0] = new XEMModelType(Binary_p_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_p_Lk_Bk);
      _parameterComponent[0] = new XEMBinaryEjParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_p_Ekj_L_B:
    {
      _parameterModelType[0] = new XEMModelType(Binary_p_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_p_L_B);
      _parameterComponent[0] = new XEMBinaryEkjParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_p_Ekj_Lk_B:
    {
      _parameterModelType[0] = new XEMModelType(Binary_p_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_p_Lk_B);
      _parameterComponent[0] = new XEMBinaryEkjParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_p_Ekj_L_Bk:
    {
      _parameterModelType[0] = new XEMModelType(Binary_p_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_p_L_Bk);
      _parameterComponent[0] = new XEMBinaryEkjParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_p_Ekj_Lk_Bk:
    {
      _parameterModelType[0] = new XEMModelType(Binary_p_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_p_Lk_Bk);
      _parameterComponent[0] = new XEMBinaryEkjParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_p_Ekjh_L_B:
    {
      _parameterModelType[0] = new XEMModelType(Binary_p_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_p_L_B);
      _parameterComponent[0] = new XEMBinaryEkjhParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_p_Ekjh_Lk_B:
    {
      _parameterModelType[0] = new XEMModelType(Binary_p_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_p_Lk_B);
      _parameterComponent[0] = new XEMBinaryEkjhParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_p_Ekjh_L_Bk:
    {
      _parameterModelType[0] = new XEMModelType(Binary_p_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_p_L_Bk);
      _parameterComponent[0] = new XEMBinaryEkjhParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_p_Ekjh_Lk_Bk:
    {
      _parameterModelType[0] = new XEMModelType(Binary_p_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_p_Lk_Bk);
      _parameterComponent[0] = new XEMBinaryEkjhParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_pk_E_L_B:
    {
      _parameterModelType[0] = new XEMModelType(Binary_pk_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_pk_L_B);
      _parameterComponent[0] = new XEMBinaryEParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_pk_E_Lk_B:
    {
      _parameterModelType[0] = new XEMModelType(Binary_pk_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_pk_Lk_B);
      _parameterComponent[0] = new XEMBinaryEParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_pk_E_L_Bk:
    {
      _parameterModelType[0] = new XEMModelType(Binary_pk_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_pk_L_Bk);
      _parameterComponent[0] = new XEMBinaryEParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_pk_E_Lk_Bk:
    {
      _parameterModelType[0] = new XEMModelType(Binary_pk_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_pk_Lk_Bk);
      _parameterComponent[0] = new XEMBinaryEParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_pk_Ek_L_B:
    {
      _parameterModelType[0] = new XEMModelType(Binary_pk_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_pk_L_B);
      _parameterComponent[0] = new XEMBinaryEkParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_pk_Ek_Lk_B:
    {
      _parameterModelType[0] = new XEMModelType(Binary_pk_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_pk_Lk_B);
      _parameterComponent[0] = new XEMBinaryEkParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_pk_Ek_L_Bk:
    {
      _parameterModelType[0] = new XEMModelType(Binary_pk_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_pk_L_Bk);
      _parameterComponent[0] = new XEMBinaryEkParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_pk_Ek_Lk_Bk:
    {
      _parameterModelType[0] = new XEMModelType(Binary_pk_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_pk_Lk_Bk);
      _parameterComponent[0] = new XEMBinaryEkParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_pk_Ej_L_B:
    {
      _parameterModelType[0] = new XEMModelType(Binary_pk_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_pk_L_B);
      _parameterComponent[0] = new XEMBinaryEjParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_pk_Ej_Lk_B:
    {
      _parameterModelType[0] = new XEMModelType(Binary_pk_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_pk_Lk_B);
      _parameterComponent[0] = new XEMBinaryEjParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_pk_Ej_L_Bk:
    {
      _parameterModelType[0] = new XEMModelType(Binary_pk_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_pk_L_Bk);
      _parameterComponent[0] = new XEMBinaryEjParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_pk_Ej_Lk_Bk:
    {
      _parameterModelType[0] = new XEMModelType(Binary_pk_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_pk_Lk_Bk);
      _parameterComponent[0] = new XEMBinaryEjParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_pk_Ekj_L_B:
    {
      _parameterModelType[0] = new XEMModelType(Binary_pk_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_pk_L_B);
      _parameterComponent[0] = new XEMBinaryEkjParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_pk_Ekj_Lk_B:
    {
      _parameterModelType[0] = new XEMModelType(Binary_pk_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_pk_Lk_B);
      _parameterComponent[0] = new XEMBinaryEkjParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_pk_Ekj_L_Bk:
    {
      _parameterModelType[0] = new XEMModelType(Binary_pk_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_pk_L_Bk);
      _parameterComponent[0] = new XEMBinaryEkjParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_pk_Ekj_Lk_Bk:
    {
      _parameterModelType[0] = new XEMModelType(Binary_pk_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_pk_Lk_Bk);
      _parameterComponent[0] = new XEMBinaryEkjParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_pk_Ekjh_L_B:
    {
      _parameterModelType[0] = new XEMModelType(Binary_pk_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_pk_L_B);
      _parameterComponent[0] = new XEMBinaryEkjhParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_pk_Ekjh_Lk_B:
    {
      _parameterModelType[0] = new XEMModelType(Binary_pk_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_pk_Lk_B);
      _parameterComponent[0] = new XEMBinaryEkjhParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_pk_Ekjh_L_Bk:
    {
      _parameterModelType[0] = new XEMModelType(Binary_pk_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_pk_L_Bk);
      _parameterComponent[0] = new XEMBinaryEkjhParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    case Heterogeneous_pk_Ekjh_Lk_Bk:
    {
      _parameterModelType[0] = new XEMModelType(Binary_pk_E);
      _parameterModelType[1] = new XEMModelType(Gaussian_pk_Lk_Bk);
      _parameterComponent[0] = new XEMBinaryEkjhParameter(_model,_parameterModelType[0],tabNbModality);
      _parameterComponent[1] = new XEMGaussianDiagParameter(_model,_parameterModelType[1]);
      break;
    }
    default:
      THROW(XEMMultinomialException,wrongModelName);
  }

}
void XEMCompositeParameter::getAllPdf(double** tabFik,double* tabProportion)const
{
  int64_t nbSample = _model->getNbSample();
  int64_t i;
  int64_t k;

   for(i=0 ; i<nbSample ; i++){
     for(k=0 ; k<_nbCluster ; k++){
       tabFik[i][k] = getPdf(i,k) * tabProportion[k];
     }
   }
}

double XEMCompositeParameter::getPdf(int64_t iSample,int64_t Kcluster) const
{
  return _parameterComponent[1]->getPdf(iSample,Kcluster)*_parameterComponent[0]->getPdf(iSample,Kcluster);
}

double XEMCompositeParameter::getPdf(XEMSample* x,int64_t kcluster) const
{
  XEMSample * GSample_ = x->getGaussianSample();
  assert(GSample_!=NULL);
  XEMSample * BSample_ = x->getBinarySample();
  assert(BSample_!=NULL);
  return _parameterComponent[1]->getPdf(GSample_,kcluster)*(_parameterComponent[0]->getPdf(BSample_,kcluster));
}

void XEMCompositeParameter::MStep()
{
  //Tab proportion Update
  computeTabProportion();
  //Binary M-step
  _parameterComponent[0]->MStep();
  //Gaussian M-step
  _parameterComponent[1]->MStep();

#ifndef NDEBUG
  double * binarytabprop_ = _parameterComponent[0]->getTabProportion();
  double * gaussiantabprop_ = _parameterComponent[1]->getTabProportion();
  for (int i = 0; i < _nbCluster; ++i) {
    assert(binarytabprop_[i]==gaussiantabprop_[i]);
  }
#endif
}

XEMCompositeParameter * XEMCompositeParameter::clone() const
{
  XEMCompositeParameter * newparam = new XEMCompositeParameter(this);
  return newparam;
}

int64_t XEMCompositeParameter::getFreeParameter() const
{
  int64_t freeparam = _parameterComponent[1]->getFreeParameter()+_parameterComponent[0]->getFreeParameter()-(_nbCluster-1);
  return freeparam;
}

void XEMCompositeParameter::initForInitRANDOM()
{
  _parameterComponent[0]->initForInitRANDOM();
  _parameterComponent[1]->initForInitRANDOM();
}

void XEMCompositeParameter::updateForInitRANDOMorUSER_PARTITION(XEMSample**tabSampleForInit,bool*tabClusterToInitialize)
{
  _parameterComponent[0]->updateForInitRANDOMorUSER_PARTITION(tabSampleForInit,tabClusterToInitialize);
  _parameterComponent[1]->updateForInitRANDOMorUSER_PARTITION(tabSampleForInit,tabClusterToInitialize);
}

void XEMCompositeParameter::recopy(XEMParameter* other)
{
  _parameterComponent[0]->recopy(other);
  _parameterComponent[1]->recopy(other);
}

void XEMCompositeParameter::reset()
{
  _parameterComponent[0]->reset();
  _parameterComponent[1]->reset();
}

void XEMCompositeParameter::updateForCV(XEMModel* originalModel,XEMCVBlock&CVBlock)
{
  _parameterComponent[0]->updateForCV(originalModel,CVBlock);
  _parameterComponent[1]->updateForCV(originalModel,CVBlock);
}

void XEMCompositeParameter::initUSER(XEMParameter * iParam)
{
  double * iTabProportion = iParam->getTabProportion();
  for (int k=0; k<_nbCluster; k++){
    // proportion (no respecting model type)
    if (!hasFreeProportion(_modelType->_nameModel)){
      _tabProportion[k] = 1.0/_nbCluster;
    }
    else{
      _tabProportion[k] = iTabProportion[k];
    }
  }
  _parameterComponent[0]->initUSER(iParam);
  _parameterComponent[1]->initUSER(iParam);
}

void XEMCompositeParameter::edit()
{
#ifdef DEBUG
  cout<<"Binary Parameters\n";
  cout<<"**********************************************************\n";
  _parameterComponent[0]->edit();
  cout<<"\nGaussian Parameters\n";
  cout<<"**********************************************************\n";
  _parameterComponent[1]->edit();
#endif
}

void XEMCompositeParameter::edit(std::ofstream & oFile, bool text)
{
#ifdef DEBUG
  _parameterComponent[0]->edit(oFile,text);
  _parameterComponent[1]->edit(oFile,text);
#endif
}

void XEMCompositeParameter::setModel(XEMModel * iModel) {
  _model = iModel;
  _parameterComponent[0]->setModel(iModel);
  _parameterComponent[1]->setModel(iModel);
}
