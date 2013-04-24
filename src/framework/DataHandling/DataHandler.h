#ifndef DATAHANDLER_H_
#define DATAHANDLER_H_
/**@file DataHandler.h
* @brief 
*/
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;
//Data Handler using singleton design pattern
class DataHandler{
  public:
    static DataHandler* getInstance();
    void readDataFromFile(std::string filename,char sep);
    void readModalityFromFile(std::string filename,char sep);
    std::vector<int> colIndex(char id);
    int nbSamples(){return nbSample_;}
    std::vector<std::vector<std::string> >& completeData(){return completedata_;}
    std::vector<std::vector<std::string> >& allModalities(){return allmodalities_;}
    ~DataHandler();
  protected:
    DataHandler(){};
    std::vector<std::vector<std::string> > completedata_;
    std::vector<std::vector<std::string> > allmodalities_;
    static DataHandler* p_instance_;
    int nbSample_;
};

#endif /* DATAHANDLER_H_ */
