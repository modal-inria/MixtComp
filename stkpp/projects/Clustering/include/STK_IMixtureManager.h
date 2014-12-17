/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2014  Serge Iovleff

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
 * Project:  stkpp::Clustering
 * created on: 15 mars 2014
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_IMixtureManager.h
 *  @brief In this file we define the Interface IMixtureManager class.
 **/


#ifndef STK_IMIXTUREMANAGER_H
#define STK_IMIXTUREMANAGER_H

#include "Arrays/include/STK_Array2D.h"

namespace STK
{
class MixtureComposer;
class IMixtureData;
class IMixture;
class IDataHandler;

/** @ingroup Clustering
 *  @brief A mixture manager is a factory class for injection dependency in the
 *  STK++ derived class of the IMixtureComposer.
 *
 *  It handles all the creation and initialization stuff needed by the
 *  mixture models of the stkpp library.
 */
class IMixtureManager
{
  public:
    typedef std::vector<IMixtureData*>::const_iterator ConstDataIterator;
    typedef std::vector<IMixtureData*>::iterator DataIterator;

    /** Default constructor, need an instance of a DataHandler.  */
    IMixtureManager(IDataHandler const* const p_handler);
    /** destructor */
    virtual ~IMixtureManager();
    /** Utility function allowing to find the idModel from the idData
     *  @param idData the id name of the data we want the idModel
     *  @return the idModel
     **/
    Clust::Mixture getIdModel( String const& idData) const;
    /** create a mixture and initialize it.
     *  @param idData name of the model
     *  @param nbCluster number of cluster of the model
     *  @return 0 if the idData is not find, the result of @c createMixture( idModel, idData, nbCluster)
     *  otherwise.
     **/
    IMixture* createMixture(String const& idData, int nbCluster);
    /** Utility function allowing to create and register all the STK++ mixtures
     *  defined in the handler.
     *  @param composer the composer claiming the mixtures
     *  @param nbCluster the number of clusters
     **/
    void createMixtures(MixtureComposer& composer, int nbCluster);
    /** @brief register a data manager to the IMixtureManager.
     *  For each mixture created and registered, a data manager is created
     *  and registered so that it will be deleted when the mixture itself is
     *  deleted.
     *  @param p_data a pointer on the data manager
     **/
    void registerMixtureData(IMixtureData* p_data);
    /** release a data manager from v_data_.
     *  @param idData name of the data set to release
     **/
    void releaseMixtureData(String const& idData);
    /** get the parameters from an IMixture.
     *  @param p_mixture pointer on the mixture
     *  @param data the array to return with the parameters
     **/
    virtual void getParameters(IMixture* p_mixture, Array2D<Real>& data) const =0;

  protected:
    /** Utility lookup function allowing to find a MixtureData from its idData
     *  @param idData the id name of the mixture we want to get
     *  @return a pointer on the MixtureData
     **/
    IMixtureData* getMixtureData( String const& idData) const;

  private:
    /** create a concrete mixture and initialize it.
     *  @param idModel Id name of the model
     *  @param idData Id name of the data
     *  @param nbCluster number of cluster of the model
     **/
    virtual IMixture* createMixtureImpl(Clust::Mixture idModel, String const& idData, int nbCluster) =0;
    /** A pointer on the concrete instance of the data handler */
    IDataHandler const* const p_handler_;
    /** vector of pointers to the data components */
    std::vector<IMixtureData*> v_data_;
};

} // namespace STK


#endif /* STK_IMIXTUREMANAGER_H */
