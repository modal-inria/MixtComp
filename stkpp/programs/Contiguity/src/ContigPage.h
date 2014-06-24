/** @file ContigPage.h
 *  @brief In this file we define the Page for the MixBin tutorial.
 **/

#ifndef CONTIGPAGE_H
#define CONTIGPAGE_H

#include "STKernel.h"
#include "DManager.h"

namespace tuto
{
/** @ingroup tutorial
 *  @brief Implement the IPage interface for the Contiguity analysis page.*/
class ContigPage: public STK::IPage
{
  public:
    /** constructor. */
    ContigPage();
    /** copy constructor.
     * @param page the page to copy
     **/
    ContigPage( ContigPage const& page);
    /** destructor */
    inline virtual ~ContigPage() {}
    /** @return the file name with the data */
    inline STK::String const& inputFileName() const { return inputFileName_;}
    /** @return the file name with the project data */
    inline STK::String const& outputFileName() const { return outputFileName_;}
    /** @return the file name with the project data */
    inline STK::String const& graphFileName() const { return graphFileName_;}
    /** @return the file name with the project data */
    inline int const& dimension() const { return dim_;}
    /** validate the options. */
    virtual bool validate();
    /*The clone method must always be reimplemented */
    inline virtual ContigPage* clone() const { return new ContigPage(*this); }

  private:
    /** Name of the file with the data. */
    STK::String inputFileName_;
    /** Name of the file with the projected data and the eigenvalues. */
    STK::String outputFileName_;
    /** Name of the file with the neighborhood graph. */
    STK::String graphFileName_;
    /** dimensiopn of the model */
    int dim_;
};

} // namespace tuto

#endif /* CONTIGPAGE_H */
