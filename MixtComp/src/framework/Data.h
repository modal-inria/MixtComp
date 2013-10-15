#ifndef DATA_H_
#define DATA_H_
/**@file Data.h
* @brief 
*/

#include "stkpp/include/DManager.h"
#include "stkpp/include/STKpp.h"

namespace MC
{

//
template<class type>
class Data {};


// specialization for double
template<>
class Data<double>
{
  public:
    STK::Array2D<double>& getData(char id, int& nbVar)
    {
      DataHandler* p_datahandler = DataHandler::getInstance();
      std::vector<int> colindex = p_datahandler->colIndex(id);
      nbSample_ = p_datahandler->nbSamples();
      nbVar = colindex.size();
      data_.resize(nbSample_, nbVar);
      for (int i = 0; i < nbSample_; ++i)
      {
        for (auto k : colindex)
        {
          data_(i,k) = STK::stringToType<double>(p_datahandler->completeData()(i,k));}
        }
      return data_;
    }

    ~Data(){}
  protected:
    STK::Array2D<double> data_;
    int nbSample_;
};



template<>
class Data<int>
{
  public:
    std::vector<int> getModality(char id) const
    {
      DataHandler* p_datahandler = DataHandler::getInstance();
      std::vector<std::vector<std::string> > allmodalities = p_datahandler->allModalities();
      std::vector<int> modality;
      for (int i = 0; i < allmodalities.size(); ++i)
      {
        if(*allmodalities[i][0].c_str() == id)
        {
          modality.resize(allmodalities[i].size()-1);
          for (int k = 0; k < modality.size(); ++k)
          {
            modality[k] = atoi(allmodalities[i][k+1].c_str());
          }
          break;
        }
      }
      return modality;
    }

    STK::Array2D<int>& getData(char id,int& nbVar)
    {
      DataHandler* p_datahandler = DataHandler::getInstance();
      std::vector<int> colindex = p_datahandler->colIndex(id);
      nbSamples_ = p_datahandler->nbSamples();
      nbVar = colindex.size();
      data_.resize(nbSamples_,nbVar);
      for (int i = 0; i < nbSamples_; ++i)
      {
        for (int k : colindex)
        { data_(i,k) = STK::stringToType<int>(p_datahandler->completeData()(i,k));}
      }
      return data_;
    }
    ~Data(){}
  protected:
    STK::Array2D<int> data_;
    int nbSamples_;
};

template<>
class Data<bool>
{
  public:
    STK::Array2D<bool>& getData(char id,int& nbVar)
    {
      DataHandler* p_datahandler = DataHandler::getInstance();
      std::vector<int> colindex = p_datahandler->colIndex(id);
      nbSamples_ = p_datahandler->nbSamples();
      nbVar = colindex.size();
      data_.resize(nbSamples_,nbVar);
      for (int i = 0; i < nbSamples_; ++i)
      {
        for (int k : colindex)
        { data_(i,k) = STK::stringToType<bool>(p_datahandler->completeData()(i,k));}
      }
      return data_;
    }

    ~Data(){}
  protected:
    STK::Array2D<bool> data_;
    int nbSamples_;
};

template<>
class Data<std::string>
{
  public:
    std::string** getData(char id,int& nbVar)
    {
      DataHandler* p_datahandler = DataHandler::getInstance();
      std::vector<int> colindex = p_datahandler->colIndex(id);
      nbSamples_ = p_datahandler->nbSamples();
      nbVar = colindex.size();
      data_ = new std::string*[nbSamples_];
      for (int i = 0; i < nbSamples_; ++i)
      {
        data_[i] = new std::string[nbVar];
        for (int k : colindex)
        {
          data_[i][k] = p_datahandler->completeData()(i,k);
        }
      }
      return data_;
    }

    ~Data(){
      for (int i = 0; i < nbSamples_; ++i) {
        delete[] data_[i];
      }
      delete[] data_;
    }
  protected:
    std::string** data_;
    int nbSamples_;
};

}

#endif /* DATA_H_ */
