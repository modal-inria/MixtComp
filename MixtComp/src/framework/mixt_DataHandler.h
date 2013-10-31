#ifndef DATAHANDLER_H_
#define DATAHANDLER_H_
/**@file DataHandler.h
* @brief 
*/
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "../stkpp/include/DManager.h"

namespace mixt
{

class DataHandler
{
  public:
    DataHandler();
    bool readDataFromFile(std::string filename,char sep);
    bool readModalityFromFile(std::string filename,char sep);
    std::vector<int> colIndex(char id);
    int nbSamples() const {return nbSample_;}
    /** get complete data set.*/
    STK::ReadWriteCsv const& completeData() const {return completedata_;}
    std::vector<std::vector<std::string> >& allModalities(){return allmodalities_;}
    ~DataHandler();
  protected:
    STK::ReadWriteCsv completedata_;
    std::vector<std::vector<std::string> > allmodalities_;
    int nbSample_;
};

} // namespace mixt

#endif /* DATAHANDLER_H_ */
