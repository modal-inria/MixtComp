#ifndef DATAHANDLING_H_
#define DATAHANDLING_H_
/**@file DataHandling.h
 * @brief 
 */
#include <string>
using namespace std;
class DataHandling
{
  public:
    DataHandling();
    static vector<vector<string> > readDataFromFile(string filename,char sep);
    virtual ~DataHandling();
};

#endif /* DATAHANDLING_H_ */
