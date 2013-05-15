#include <iostream>
#include "DataHandler.h"


DataHandler* DataHandler::p_instance_ = NULL;

DataHandler::~DataHandler(){
}

DataHandler* DataHandler::getInstance(){
  if(p_instance_ == NULL)
    p_instance_ = new DataHandler();

  return p_instance_;
}

std::vector<int> DataHandler::colIndex(char id){
  std::vector<int> colindex;
  for (int i = 0; i < completedata_[0].size(); ++i) {
    if(*completedata_[0][i].c_str()==id)
      colindex.push_back(i);
  }
  return colindex;
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
  completedata_ = std::vector<std::vector<std::string> >(temprows,std::vector<std::string>(tempcols));
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

void DataHandler::readModalityFromFile(std::string filename,char sep)
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

  allmodalities_ = std::vector<std::vector<std::string> >(temprows,std::vector<std::string>(tempcols));
  temprows = 0;
  file.close();
  file.open(filename.c_str(),ios::in);
  while( getline(file,dataline) ){
    istringstream csvStream(dataline);
    int idx = 0;
    while( getline(csvStream,allmodalities_[temprows][idx],sep) ){
      idx++;
    }
    temprows++;
  }
}
