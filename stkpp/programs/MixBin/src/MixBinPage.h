/** @file MixBinPage.h
 *  @brief In this file we define the Page for the MixBin tutorial.
 **/

#ifndef MIXBINPAGE_H
#define MIXBINPAGE_H

#include "STKernel.h"
#include "DManager.h"
#include <map>

namespace tuto
{
/** @ingroup tutorial
 *  @brief Implement the IPage interface for the Mixture Binary model page.*/
class MixBinPage: public STK::IPage
{
  public:
    /** constructor. */
    MixBinPage();
    /** copy constructor.
     * @param page the page to copy
     **/
    MixBinPage( MixBinPage const& page);
    /** destructor */
    inline virtual ~MixBinPage() {}
    /** @return the file name with the data */
    inline STK::String const& fileName() const { return fileName_;}
    /** @return the maping Binary -> String. */
    inline std::map<STK::Binary, STK::String> const& mapping() const { return mapping_;}
    /** @return the range of the models to estimate */
    inline STK::Range const& dims() const { return dims_;}
    /** validate the options. If mapping is present overwrite ampping_ with the values
     *  given */
    virtual bool validate();
    /*The clone method must always be reimplemented */
    inline virtual IPage* clone() const { return new MixBinPage(*this); }
  private:
    /** Name of the file with the data. */
    STK::String fileName_;
    /** Name of the file with the data. */
    std::map<STK::Binary, STK::String> mapping_;
    /** dimensions of the models. */
    STK::Range dims_;
};

} // namespace tuto

#endif /* MIXBINPAGE_H */
