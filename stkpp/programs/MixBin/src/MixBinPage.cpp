/** @file MixBinPage.cpp
 *  @brief In this file we implement the MixBinPage class.
 **/

#include "MixBinPage.h"
using namespace STK;

namespace tuto
{

/* constructor. */
MixBinPage::MixBinPage() : IPage(_T("MixBin"), 1, false)
                         , fileName_()
                         , mapping_()
                         , dims_(2,1)  // dims_=2:2
{
  mapping_.insert(std::pair<Binary, String>(zero_, String(_T("0"))));
  mapping_.insert(std::pair<Binary, String>(one_, String(_T("1"))));
  mapping_.insert(std::pair<Binary, String>(binaryNA_, stringNa));

  options_.reserve(3);
  options_.push_back(Option(_T("filename"), Option::string_, false));
  options_.push_back(Option(_T("mapping"), Option::lstring_, true));
  options_.push_back(Option(_T("dims"), Option::range_, true));
  options_.back().setValue(typeToString(dims_));
}
/* copy constructor.
 * @param page the page to copy
 **/
MixBinPage::MixBinPage( MixBinPage const& page)
                      : IPage(page)
                      , fileName_(page.fileName_)
                      , mapping_(page.mapping_)
                      , dims_(page.dims_)
{}

/* validate the options. If mapping is present overwrite ampping_ with the values
 *  given */
bool MixBinPage::validate()
{
  fileName_ = options_[0].get(String());
  dims_ = options_[2].get(Range());
  std::list<String> const& coding = options_[1].get(std::list<String>());
  if (coding.size()>3)
  { msg_error_ = _T("Error in MixBin Page. Only 3 string max for coding a Binary.");
    return false;
  }
  // read coding values
  typename std::list<String>::const_iterator it = coding.begin();
  if (coding.size()>0) mapping_[zero_] = *it; ++it;
  if (coding.size()>1) mapping_[one_]  = *it; ++it;
  if (coding.size()>2) mapping_[binaryNA_] = *it;
  return true;
}

} /* namespace tuto */
