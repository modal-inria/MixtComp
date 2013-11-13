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
    ~DataHandler();

    bool readDataFromFile(std::string filename,char sep);
    bool readModalityFromFile(std::string filename,char sep);
    std::vector<int> colIndex(char id) const;
    int nbSamples() const {return nbSample_;}
    /** get complete data set.*/
    STK::ReadWriteCsv const& completeData() const {return completedata_;}
    std::vector<std::vector<std::string> >& allModalities(){return allmodalities_;}

    template<typename Data> void getData(Data& data, char id) const;

  protected:
    STK::ReadWriteCsv completedata_;
    std::vector<std::vector<std::string> > allmodalities_;
    int nbSample_;
};

// specialization for STK::Array2D<STK::Real>
template<>
void DataHandler::getData<STK::Array2D<STK::Real>>(STK::Array2D<STK::Real>& data, char id) const
{
  std::vector<int> colindex = colIndex(id);
  data.resize(nbSample_, colindex.size());
  for (int i = 0; i < nbSample_; ++i)
  {
    for (auto k : colindex)
    {
      data(i,k) = STK::stringToType<double>(completeData()(i,k));
    }
  }
}

} // namespace mixt

#endif /* DATAHANDLER_H_ */
