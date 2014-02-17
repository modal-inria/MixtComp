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
 * created on: 7 nov. 2009
 * Purpose:  .
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Category.h In this file we define the Category class
 *  @brief A Category is a set of of Tool.
 **/

#ifndef CATEGORY_H
#define CATEGORY_H

#include "../../../projects/STKernel/include/STK_String.h"
#include "../../../projects/DManager/include/STK_List1D.h"

#include "STK_Tool.h"

namespace STK
{

/**
 * @brief The Category class specifies a category in FreeMiner's tool box
 * component.
 * Each Category contain a set of Tool with similar properties.
 **/
class Category
{
  public:
    /**
     * Type of the category
     */
    enum Type
    {
      Default,  ///< Default : the category is frozen
      Scratchpad///< Scratchpad : the category can be modified
    };

  public:
    /**
     * Constructor
     * @param aname the name of the Category
     * @param atype the Type of the Category
     */
    Category( String const& aname = String(), Type atype = Default);
    /**
     * Destructor
     **/
   virtual ~Category();
    /**
     *  Give the name of the Category
     * @return the name of the Category
     */
    inline String const& name() const
    { return name_;}
    /**
     *  Set the name of the Category
     * @param aname the name to set
     */
    inline void setName( String const& aname)
    { name_ = aname;}
    /**
     * count the number fo Tool in the Category
     * @return the number of Toool in the Category
     */
    inline int toolCount() const
    { return tool_list_.size();}
    /**
     * Return a copy of the Tool with the given index
     * @param idx the index of the tool
     * @return the tool with the given index
     */
    inline Tool tool( int idx) const
    { return tool_list_.elt(idx);}
    /**
     * remove the Tool with the given index
     * @param idx the index of the Tool to remove
     */
    inline void removeTool( int idx)
    { tool_list_.erase(idx);}
    /**
     * Add a Tool to the Category
     * @param atool the Tool to add
     */
    inline void addTool( const Tool &atool)
    { tool_list_.push_back(atool);}
    /**
     * Give the Type of the Category
     * @return the Type of the Category
     */
    inline Type type() const
    { return type_;}
    /**
     * Set the Type of the Category
     * @param atype the Type to set
     */
    inline void setType( Type atype)
    { type_ = atype;}
    /**
     * @return true if the category does not have a name
     */
    inline bool isNull() const
    { return name_.empty();}

  private:
    /**
     * name of the Category
     */
    String name_;
    /**
     * type of the Category
     */
    Type type_;
    /**
     * list of the Tool in the Category
     */
    List1D<Tool> tool_list_;

};

} // namespace STK

#endif /* CATEGORY_H */
