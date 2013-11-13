#include <iostream>
#include "mixt_DataHandler.h"

namespace mixt
{

DataHandler::DataHandler()
  :nbSample_(0)
{
}

DataHandler::~DataHandler(){
}

std::vector<int> DataHandler::colIndex(char id) const
{
  std::vector<int> colindex;
  for (int i = 0; i < completedata_.size(); ++i)
  {
    if (completedata_.var(i).name() == STK::String(1,id))
      colindex.push_back(i);
  }
  return colindex;
}

bool DataHandler::readDataFromFile(std::string filename,char sep)
{
  STK::ReadWriteCsv rw(filename, true, STK::String(1,sep));
  if (!rw.read())
  {
    return false;
  }
  completedata_ += rw;
  nbSample_ = completedata_.rows().size();

  return true;
}

bool DataHandler::readModalityFromFile(std::string filename,char sep)
{
  std::fstream file;
  file.open(filename.c_str());
  if (!file.is_open()) {
    std::cerr<<"Cannot open Modality file\n";
    return false;
  }
  std::string dataline,dataval;
  int temprows = 0, tempcols = 0;
  std::istringstream datalineStream;

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
  file.open(filename.c_str(),std::ios::in);
  while( getline(file,dataline) ){
    std::istringstream csvStream(dataline);
    int idx = 0;
    while( getline(csvStream,allmodalities_[temprows][idx],sep) ){
      idx++;
    }
    temprows++;
  }

  return true;
}

} // namespace mixt
