/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2007  Serge Iovleff

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as
 published by the Free Software Foundation; either version 2 of the
 License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this program; if not, write to the
 Free Software Foundation, Inc.,
 59 Temple Place,
 Suite 330,
 Boston, MA 02111-1307
 USA

 Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 */

/*
 * Project:  stkpp::gui::model
 * created on: 19 oct. 2009
 * Purpose:  .
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_GProgram.h
 *  @brief In this file we declare the GProgram class
 **/

#ifndef STK_GPROGRAM_H
#define STK_GPROGRAM_H

#include <list>

#include "STK_IPropertySource.h"

namespace STK
{

class IStep;

/** @brief A GProgram is a succession of Step connected.
 *  Each Step Perform a specific treatment.
 */
class GProgram : public IPropertySource
{
  private:
    /** Property ID for the name of the GProgram value.  */
    static const String ID_GPROGAM_NAME;

    /** Descriptor of the name property */
    static const PropertyDescriptor nameDescriptor_;

    /** name of the GProgram */
    String name_;

    /** list of the steps */
   std::list<IStep*> steps_;

  public:
    /** constructor */
    GProgram(String const& name);

    /** destructor */
    virtual ~GProgram();

    /** get the name_ of the program.
     * @return the name of the program
     */
    inline String const& name()
    {return name_;}

    /** Execute the program */
    void run();

    /** Add a step to the Program.
     * @param step The step to add
     **/
    void addStep(IStep* step);

    /** remove a step to the Program.
     * @param step The step to remove
     **/
    void removeStep(IStep* step);

    /** @brief Add a Connection between two steps of the Program.
     * addConnection check if the Connection will not create an infinite loop
     * in the program. Return @c false if it is the case.
     * <p> The method
     * will ask to the target Step if the Connection is compatible. If the
     * Connection is compatible the Connection is created and added to the
     * list of incoming Connection to the target Step and the outgoing
     * Connection of the source Step
     * </p>
     * @see IStep#addConnection
     *
     * @param source The source of the Connection
     * @param target The target of the Connection
     * @return true if the Connection have been added
     **/
    bool addConnection(IStep* source, IStep* target);

    /** @brief Implementation of the pure virtual method
     *  <code> isPropertySet</code> defined in <code> IPropertySource</code>
     * @see IPropertySource#isPropertySet
     *
     * @param id the id of the property
     * @return <code>true</code> if id is a valid IStep id property.
     */
    virtual bool isPropertySet(String const& id) const;

    /** @brief Implementation of the pure virtual method
     *  <code> getPropertyValue</code> defined in <code> IPropertySource</code>
     * @see IPropertySource#getPropertValue
     *
     * @param id the id of the property
     * @return the value of the property, or <code>NA</code>
     */
    virtual const String* getPropertyValue(String const& id) const;
};

} // namespace STK

#endif /* STK_GPROGRAM_H */
