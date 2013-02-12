#include "IDeveloper.h"

IDeveloper::IDeveloper()
{
  // TODO Auto-generated constructor stub

}

IDeveloper::~IDeveloper()
{
  // TODO Auto-generated destructor stub
}


vector<vector<string> > IDeveloper::readDataFromFile(string filename,char sep)
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
  while(getline(csvStream,vdataval,sep)){
    tempcols++;
  }
  //start counting rows after first row
  temprows++;
  while( getline(file, dataline) ){
    temprows++;
  }

  vector<vector<string> > data(temprows,tempcols);
  temprows = 0;
  file.close();
  file.open(filename.c_str(),ios::in);
  while( getline(file, csvLine) ){
    istringstream csvStream(csvLine);
    int idx = 0;
    while( getline(csvStream,data[temprows][idx], sep) ){
      idx++;
    }
    temprows++;
  }

  return data;
}
