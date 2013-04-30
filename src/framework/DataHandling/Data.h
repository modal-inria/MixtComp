#ifndef DATA_H_
#define DATA_H_
/**@file Data.h
* @brief 
*/

#include "DataHandler.h"
template<class type>
class Data{
};
template<>
class Data<double>{
  public:
    std::vector<std::vector<double> >& getData(char id,int& variables_){
      DataHandler* p_datahandler = DataHandler::getInstance();
      std::vector<int> colindex = p_datahandler->colIndex(id);
      samples_ = p_datahandler->nbSamples();
      variables_ = colindex.size();
      std::vector<std::vector<std::string> > completedata = p_datahandler->completeData();
      data_ = std::vector<std::vector<double> >(samples_,std::vector<double>(variables_));
      for (int i = 0; i < samples_; ++i) {
        for (int k : colindex) {
          data_[i][k] = atof(completedata[i][k].c_str());
        }
      }
      return data_;
    }

    ~Data(){}
  protected:
    std::vector<std::vector<double> > data_;
    int samples_;
};



template<>
class Data<int>{
  public:
      std::vector<int> getModality(char id){
        DataHandler* p_datahandler = DataHandler::getInstance();
        std::vector<std::vector<std::string> > allmodalities = p_datahandler->allModalities();
        for (int i = 0; i < allmodalities.size(); ++i) {
          if(*allmodalities[i][0].c_str() == id){
            std::vector<int> modality(allmodalities[i].size()-1);
            for (int k = 1; k <= modality.size(); ++k) {
              modality[k-1] = atoi(allmodalities[i][k].c_str());
            }
            return modality;
          }
        }

      }

      std::vector<std::vector<int> >& getData(char id,int& variables_){
      DataHandler* p_datahandler = DataHandler::getInstance();
      std::vector<int> colindex = p_datahandler->colIndex(id);
      samples_ = p_datahandler->nbSamples();
      variables_ = colindex.size();
      std::vector<std::vector<std::string> > completedata = p_datahandler->completeData();
      data_ = std::vector<std::vector<int> >(samples_,std::vector<int>(variables_));
      for (int i = 1; i <= samples_; ++i) {
        for (int k : colindex) {
          data_[i-1][k] = atoi(completedata[i][k].c_str());
        }
      }
      return data_;
    }
    ~Data(){}
  protected:
    std::vector<std::vector<int> > data_;
    int samples_;
};

template<>
class Data<bool>{
  public:
      std::vector<std::vector<bool> >& getData(char id,int& variables_){
      DataHandler* p_datahandler = DataHandler::getInstance();
      std::vector<int> colindex = p_datahandler->colIndex(id);
      samples_ = p_datahandler->nbSamples();
      variables_ = colindex.size();
      std::vector<std::vector<std::string> > completedata = p_datahandler->completeData();
      data_ = std::vector<std::vector<bool> >(samples_,std::vector<bool>(variables_));;
      for (int i = 0; i < samples_; ++i) {
        for (int k : colindex) {
          data_[i][k] = atoi(completedata[i][k].c_str());
        }
      }
      return data_;
    }

    ~Data(){}
  protected:
    std::vector<std::vector<bool> > data_;
    int samples_;
};

template<>
class Data<std::string>{
  public:
    std::string** getData(char id,int& variables_){
      DataHandler* p_datahandler = DataHandler::getInstance();
      std::vector<int> colindex = p_datahandler->colIndex(id);
      samples_ = p_datahandler->nbSamples();
      variables_ = colindex.size();
      std::vector<std::vector<std::string> > completedata = p_datahandler->completeData();
      data_ = new std::string*[samples_];
      for (int i = 0; i < samples_; ++i) {
        data_[i] = new std::string[variables_];
        for (int k : colindex) {
          data_[i][k] = completedata[i][k];
        }
      }
      return data_;
    }

    ~Data(){
      for (int i = 0; i < samples_; ++i) {
        delete[] data_[i];
      }
      delete[] data_;
    }
  protected:
    std::string** data_;
    int samples_;
};

#endif /* DATA_H_ */
