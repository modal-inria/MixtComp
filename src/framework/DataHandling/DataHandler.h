#ifndef DATAHANDLER_H_
#define DATAHANDLER_H_
/**@file DataHandler.h
* @brief 
*/

#endif /* DATAHANDLER_H_ */
#include <string>
#include <fstream>
#include <sstream>
using namespace std;
//Data Handler using singleton design pattern
class DataHandler{
  public:
    static DataHandler* getInstance();
    void readDataFromFile(std::string filename,char sep);
    std::vector<int> colIndex(char id);
    int nbSamples(){return nbSample_;}
    std::string** completeData(){return completedata_;}
    ~DataHandler();
  protected:
    static std::string** completedata_;
    static DataHandler* p_instance_;
    int nbSample_;
    DataHandler();
};


DataHandler* DataHandler::getInstance(){
  if(p_instance_ == NULL)
    p_instance_ = new DataHandler();

  return p_instance_;
}
void DataHandler::readDataFromFile(std::string filename,char sep)
{
  fstream file;
  file.open(filename.c_str());
  if (!file.is_open()) {
    // TODO : throw exception
  }
  string dataline,dataval;
  int temprows = 0, tempcols = 0;
  istringstream datalineStream;

  //get first line of input file to count number of columns
  getline(file, dataline);
  datalineStream.str(dataline);
  while(getline(datalineStream,dataval,sep)){
    tempcols++;
  }

  temprows++;
  //start counting rows after first row
  while( getline(file, dataline) ){
    temprows++;
  }

  nbSample_ = temprows-1; //total number of individuals
  completedata_ = new std::string*[temprows];
  for (int i = 0; i < temprows; ++i) {
    completedata_[i] = new std::string[tempcols];
  }
  temprows = 0;
  file.close();
  file.open(filename.c_str(),ios::in);
  while( getline(file,dataline) ){
    istringstream csvStream(dataline);
    int idx = 0;
    while( getline(csvStream,completedata_[temprows][idx],sep) ){
      idx++;
    }
    temprows++;
  }
}

template<class type>
class Data{
};
template<>
class Data<double>{
  public:
    double** getData(char id){
      DataHandler* p_datahandler = DataHandler::getInstance();
      std::vector<int> colindex = p_datahandler->colIndex(id);
      samples_ = p_datahandler->nbSamples();
      variables_ = colindex.size();
      std::string** completedata = p_datahandler->completeData();
      data_ = new double*[samples_];
      for (int i = 0; i < samples_; ++i) {
        data_[i] = new double[variables_];
        for (int k : colindex) {
          data_[i][k] = atof(completedata[i][k].c_str());
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
    double** data_;
    int samples_,variables_;
};



template<>
class Data<int>{
  public:
    int** getData(char id){
      DataHandler* p_datahandler = DataHandler::getInstance();
      std::vector<int> colindex = p_datahandler->colIndex(id);
      samples_ = p_datahandler->nbSamples();
      variables_ = colindex.size();
      std::string** completedata = p_datahandler->completeData();
      data_ = new int*[samples_];
      for (int i = 0; i < samples_; ++i) {
        data_[i] = new int[variables_];
        for (int k : colindex) {
          data_[i][k] = atoi(completedata[i][k].c_str());
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
    int** data_;
    int samples_,variables_;
};

template<>
class Data<bool>{
  public:
    bool** getData(char id){
      DataHandler* p_datahandler = DataHandler::getInstance();
      std::vector<int> colindex = p_datahandler->colIndex(id);
      samples_ = p_datahandler->nbSamples();
      variables_ = colindex.size();
      std::string** completedata = p_datahandler->completeData();
      data_ = new bool*[samples_];
      for (int i = 0; i < samples_; ++i) {
        data_[i] = new bool[variables_];
        for (int k : colindex) {
          data_[i][k] = atoi(completedata[i][k].c_str());
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
    bool** data_;
    int samples_,variables_;
};

template<>
class Data<std::string>{
  public:
    std::string** getData(char id){
      DataHandler* p_datahandler = DataHandler::getInstance();
      std::vector<int> colindex = p_datahandler->colIndex(id);
      samples_ = p_datahandler->nbSamples();
      variables_ = colindex.size();
      std::string** completedata = p_datahandler->completeData();
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
    int samples_,variables_;
};
