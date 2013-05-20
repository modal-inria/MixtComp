#ifndef DATAHANDLER_H_
#define DATAHANDLER_H_
/**@file DataHandler.h
* @brief 
*/
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "../externlibs/stkpp/include/DManager.h"

//Data Handler using singleton design pattern
class DataHandler
{
  public:
    static DataHandler* getInstance();
    void readDataFromFile(std::string filename,char sep);
    void readModalityFromFile(std::string filename,char sep);
    std::vector<int> colIndex(char id);
    int nbSamples() const {return nbSample_;}
    /** get complete data set.*/
    STK::ReadWriteCsv const& completeData() const {return completedata_;}
    std::vector<std::vector<std::string> >& allModalities(){return allmodalities_;}
    ~DataHandler();
  protected:
    DataHandler(){
      p_instance_ = NULL;
    };
    STK::ReadWriteCsv completedata_;
    std::vector<std::vector<std::string> > allmodalities_;
    static DataHandler* p_instance_;
    int nbSample_;
};

#endif /* DATAHANDLER_H_ */
