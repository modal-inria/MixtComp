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

/** @file STK_MixtureManager.h
 *  @brief In this file we define the MixtureManager class.
 **/


#ifndef STK_MIXTUREMANAGER_H
#define STK_MIXTUREMANAGER_H

#include "DManager/include/STK_IDataHandler.h"
#include "MixturesBridges/STK_MixtureBridge.h"
#include "STK_IMixtureManager.h"

#define STK_CREATE_MIXTURE(Data, Bridge) \
          Data* p_data = new Data(idData); \
          registerMixtureData(p_data); \
          p_handler_->getData(idData, p_data->m_dataij_, p_data->nbVariable_ ); \
          p_data->initialize(); \
          Bridge* p_bridge = new Bridge( p_data, idData, nbCluster);  \
          return p_bridge;

namespace STK
{
/** @ingroup Clustering
 *  @brief A mixture manager is a factory class for injection dependency in the
 *  STK++ derived class of the IMixtureComposer interface
 *  (aka: the MixtureComposer class).
 *
 *  It allow to handle all the creation and initialization stuff needed by the
 *  (bridged) mixture models of the stkpp library.
 */
template<class DataHandler>
class MixtureManager : public IMixtureManager
{
  public:
    // All data handlers will store and return a specific container for
    // the data they handle. The DataHandlerTraits class allow us to know the
    // type of these containers when data is Real and Integer.
    typedef typename hidden::DataHandlerTraits<DataHandler, Real>::Data DataReal;
    typedef typename hidden::DataHandlerTraits<DataHandler, Integer>::Data DataInt;
    // Classes wrapping the Real and Integer containers
    typedef MixtureData<DataReal> MixtureDataReal;
    typedef MixtureData<DataInt>  MixtureDataInt;

    // All Gamma bridges
    typedef MixtureBridge<Clust::Gamma_ajk_bjk_, DataReal> MixtureBridge_ajk_bjk;
    typedef MixtureBridge<Clust::Gamma_ajk_bk_,  DataReal> MixtureBridge_ajk_bk;
    typedef MixtureBridge<Clust::Gamma_ajk_bj_,  DataReal> MixtureBridge_ajk_bj;
    typedef MixtureBridge<Clust::Gamma_ajk_b_,   DataReal> MixtureBridge_ajk_b;
    typedef MixtureBridge<Clust::Gamma_ak_bjk_,  DataReal> MixtureBridge_ak_bjk;
    typedef MixtureBridge<Clust::Gamma_ak_bk_,   DataReal> MixtureBridge_ak_bk;
    typedef MixtureBridge<Clust::Gamma_ak_bj_,   DataReal> MixtureBridge_ak_bj;
    typedef MixtureBridge<Clust::Gamma_ak_b_,    DataReal> MixtureBridge_ak_b;
    typedef MixtureBridge<Clust::Gamma_aj_bjk_,  DataReal> MixtureBridge_aj_bjk;
    typedef MixtureBridge<Clust::Gamma_aj_bk_,   DataReal> MixtureBridge_aj_bk;
    typedef MixtureBridge<Clust::Gamma_a_bjk_,   DataReal> MixtureBridge_a_bjk;
    typedef MixtureBridge<Clust::Gamma_a_bk_,    DataReal> MixtureBridge_a_bk;
    // All Gaussian bridges
    typedef MixtureBridge<Clust::Gaussian_sjk_, DataReal> MixtureBridge_sjk;
    typedef MixtureBridge<Clust::Gaussian_sk_,  DataReal> MixtureBridge_sk;
    typedef MixtureBridge<Clust::Gaussian_sj_,  DataReal> MixtureBridge_sj;
    typedef MixtureBridge<Clust::Gaussian_s_,   DataReal> MixtureBridge_s;
    // All Categorical bridges
    typedef MixtureBridge<Clust::Categorical_pjk_, DataInt> MixtureBridge_pjk;
    typedef MixtureBridge<Clust::Categorical_pk_,  DataInt> MixtureBridge_pk;

    /** Default constructor, need an instance of a DataHandler.  */
    MixtureManager(DataHandler const& handler) : IMixtureManager(&handler), p_handler_(&handler) {}
    /** destructor */
    virtual ~MixtureManager() {}
    /** get the parameters from an IMixture.
     *  @param p_mixture pointer on the mixture
     *  @param param the array to return with the parameters
     **/
    void getParameters(IMixture* p_mixture, Array2D<Real>& param) const
    {
      Clust::Mixture idModel = getIdModel(p_mixture->idName());
      if (idModel == Clust::unknown_mixture_) return;
      // up-cast... (Yes it's bad....;)...)
      switch (idModel)
      {
        // gamma models
        case Clust::Gamma_ajk_bjk_:
        { static_cast<MixtureBridge_ajk_bjk const*>(p_mixture)->getParameters(param);}
        break;
        case Clust::Gamma_ajk_bk_:
        { static_cast<MixtureBridge_ajk_bk const*>(p_mixture)->getParameters(param);}
        break;
        case Clust::Gamma_ajk_bj_:
        { static_cast<MixtureBridge_ajk_bj const*>(p_mixture)->getParameters(param);}
        break;
        case Clust::Gamma_ajk_b_:
        { static_cast<MixtureBridge_ajk_b const*>(p_mixture)->getParameters(param);}
        break;
        case Clust::Gamma_ak_bjk_:
        { static_cast<MixtureBridge_ak_bjk const*>(p_mixture)->getParameters(param);}
        break;
        case Clust::Gamma_ak_bk_:
        { static_cast<MixtureBridge_ak_bk const*>(p_mixture)->getParameters(param);}
        break;
        case Clust::Gamma_ak_bj_:
        { static_cast<MixtureBridge_ak_bj const*>(p_mixture)->getParameters(param);}
        break;
        case Clust::Gamma_ak_b_:
        { static_cast<MixtureBridge_ak_b const*>(p_mixture)->getParameters(param);}
        break;
        case Clust::Gamma_aj_bjk_:
        { static_cast<MixtureBridge_aj_bjk const*>(p_mixture)->getParameters(param);}
        break;
        case Clust::Gamma_aj_bk_:
        { static_cast<MixtureBridge_aj_bk const*>(p_mixture)->getParameters(param);}
        break;
        case Clust::Gamma_a_bjk_:
        { static_cast<MixtureBridge_a_bjk const*>(p_mixture)->getParameters(param);}
        break;
        case Clust::Gamma_a_bk_:
        { static_cast<MixtureBridge_a_bk const*>(p_mixture)->getParameters(param);}
        break;
        // Gaussian models
        case Clust::Gaussian_sjk_:
        { static_cast<MixtureBridge_sjk const*>(p_mixture)->getParameters(param);}
        break;
        case Clust::Gaussian_sk_:
        { static_cast<MixtureBridge_sk const*>(p_mixture)->getParameters(param);}
        break;
        case Clust::Gaussian_sj_:
        { static_cast<MixtureBridge_sj const*>(p_mixture)->getParameters(param);}
        break;
        case Clust::Gaussian_s_:
        { static_cast<MixtureBridge_s const*>(p_mixture)->getParameters(param);}
        break;
        // Categorical models
        case Clust::Categorical_pjk_:
        { static_cast<MixtureBridge_pjk const*>(p_mixture)->getParameters(param);}
        break;
        case Clust::Categorical_pk_:
        { static_cast<MixtureBridge_pk const*>(p_mixture)->getParameters(param);}
        break;
        default: // idModel is not implemented
        break;
      }
    }
    /** get the parameters from an IMixture.
     *  @param p_mixture pointer on the mixture
     *  @return the array with the parameters
     **/
    ArrayXX getParameters(IMixture* p_mixture) const
    {
      Clust::Mixture idModel = getIdModel(p_mixture->idName());
      if (idModel == Clust::unknown_mixture_) return ArrayXX();
      // up-cast... (Yes it's bad....;)...)
      switch (idModel)
      {
        // gamma models
        case Clust::Gamma_ajk_bjk_:
        { return static_cast<MixtureBridge_ajk_bjk const*>(p_mixture)->getParameters();}
        break;
        case Clust::Gamma_ajk_bk_:
        { return static_cast<MixtureBridge_ajk_bk const*>(p_mixture)->getParameters();}
        break;
        case Clust::Gamma_ajk_bj_:
        { return static_cast<MixtureBridge_ajk_bj const*>(p_mixture)->getParameters();}
        break;
        case Clust::Gamma_ajk_b_:
        { return static_cast<MixtureBridge_ajk_b const*>(p_mixture)->getParameters();}
        break;
        case Clust::Gamma_ak_bjk_:
        { return static_cast<MixtureBridge_ak_bjk const*>(p_mixture)->getParameters();}
        break;
        case Clust::Gamma_ak_bk_:
        { return static_cast<MixtureBridge_ak_bk const*>(p_mixture)->getParameters();}
        break;
        case Clust::Gamma_ak_bj_:
        { return static_cast<MixtureBridge_ak_bj const*>(p_mixture)->getParameters();}
        break;
        case Clust::Gamma_ak_b_:
        { return static_cast<MixtureBridge_ak_b const*>(p_mixture)->getParameters();}
        break;
        case Clust::Gamma_aj_bjk_:
        { return static_cast<MixtureBridge_aj_bjk const*>(p_mixture)->getParameters();}
        break;
        case Clust::Gamma_aj_bk_:
        { return static_cast<MixtureBridge_aj_bk const*>(p_mixture)->getParameters();}
        break;
        case Clust::Gamma_a_bjk_:
        { return static_cast<MixtureBridge_a_bjk const*>(p_mixture)->getParameters();}
        break;
        case Clust::Gamma_a_bk_:
        { return static_cast<MixtureBridge_a_bk const*>(p_mixture)->getParameters();}
        break;
        // Gaussian models
        case Clust::Gaussian_sjk_:
        { return static_cast<MixtureBridge_sjk const*>(p_mixture)->getParameters();}
        break;
        case Clust::Gaussian_sk_:
        { return static_cast<MixtureBridge_sk const*>(p_mixture)->getParameters();}
        break;
        case Clust::Gaussian_sj_:
        { return static_cast<MixtureBridge_sj const*>(p_mixture)->getParameters();}
        break;
        case Clust::Gaussian_s_:
        { return static_cast<MixtureBridge_s const*>(p_mixture)->getParameters();}
        break;
        // Categorical models
        case Clust::Categorical_pjk_:
        { return static_cast<MixtureBridge_pjk const*>(p_mixture)->getParameters();}
        break;
        case Clust::Categorical_pk_:
        { return static_cast<MixtureBridge_pk const*>(p_mixture)->getParameters();}
        break;
        default: // idModel is not implemented
        break;
      }
    }
    /** get the missing values of a data set.
     *  @param idData Id name of the data set attached to the mixture
     *  @param data the array to return with the missing values
     **/
    template<typename Type>
    void getMissingValues( String const& idData, std::vector< std::pair< std::pair<int,int>, Type > >& data) const
    {
      typedef typename hidden::DataHandlerTraits<DataHandler, Type>::Data DataType;
      typedef MixtureData<DataType> MixtureDataType;

      IMixtureData* p_data = getMixtureData(idData);
      // up-cast... (Yes it's bad....;)...)
      if (p_data)
      { static_cast<MixtureDataType const*>(p_data)->getMissingValues(data);}
    }
    /** get the wrapper for any kind of data set using its Id
     *  @param idData Id name of the data set attached to the mixture
     *  @return a constant reference on the array with the data set
     **/
    template<typename Type>
    typename hidden::DataHandlerTraits<DataHandler, Type>::Data const& getData( String const& idData) const
    {
      typedef typename hidden::DataHandlerTraits<DataHandler, Type>::Data DataType;
      typedef MixtureData<DataType> MixtureDataType;
      return static_cast<MixtureDataType const*>(getMixtureData(idData))->m_dataij_;
    }

  protected:
    /** create a concrete mixture and initialize it.
     *  @param idModel Id name of the model
     *  @param idData Id name of the data
     *  @param nbCluster number of cluster of the model
     **/
    virtual IMixture* createMixtureImpl(Clust::Mixture idModel, String const& idData, int nbCluster)
    {
      switch (idModel)
      {
        // gamma_ajk_bjk_ model
        case Clust::Gamma_ajk_bjk_:
        { STK_CREATE_MIXTURE(MixtureDataReal, MixtureBridge_ajk_bjk)}
        break;
        // gamma_ajk_bk_ model
        case Clust::Gamma_ajk_bk_:
        { STK_CREATE_MIXTURE(MixtureDataReal, MixtureBridge_ajk_bk)}
        break;
        // gamma_ajk_bj_ model
        case Clust::Gamma_ajk_bj_:
        { STK_CREATE_MIXTURE(MixtureDataReal, MixtureBridge_ajk_bj)}
        break;
        // gamma_ajk_b_ model
        case Clust::Gamma_ajk_b_:
        { STK_CREATE_MIXTURE(MixtureDataReal, MixtureBridge_ajk_b)}
        break;
        // gamma_ak_bjk_ model
        case Clust::Gamma_ak_bjk_:
        { STK_CREATE_MIXTURE(MixtureDataReal, MixtureBridge_ak_bjk)}
        break;
        // gamma_ak_bk_ model
        case Clust::Gamma_ak_bk_:
        { STK_CREATE_MIXTURE(MixtureDataReal, MixtureBridge_ak_bk)}
        break;
        // gamma_ak_bj_ model
        case Clust::Gamma_ak_bj_:
        { STK_CREATE_MIXTURE(MixtureDataReal, MixtureBridge_ak_bj)}
        break;
        // gamma_ajk_b_ model
        case Clust::Gamma_ak_b_:
        { STK_CREATE_MIXTURE(MixtureDataReal, MixtureBridge_ak_b)}
        break;
        // gamma_aj_bjk_ model
        case Clust::Gamma_aj_bjk_:
        { STK_CREATE_MIXTURE(MixtureDataReal, MixtureBridge_aj_bjk)}
        break;
        // gamma_aj_bk_ model
        case Clust::Gamma_aj_bk_:
        { STK_CREATE_MIXTURE(MixtureDataReal, MixtureBridge_aj_bk)}
        // gamma_aj_bjk_ model
        case Clust::Gamma_a_bjk_:
        { STK_CREATE_MIXTURE(MixtureDataReal, MixtureBridge_a_bjk)}
        break;
        // gamma_aj_bk_ model
        case Clust::Gamma_a_bk_:
        { STK_CREATE_MIXTURE(MixtureDataReal, MixtureBridge_a_bk)}
        // Gaussian_sjk_ model
        case Clust::Gaussian_sjk_:
        { STK_CREATE_MIXTURE(MixtureDataReal, MixtureBridge_sjk)}
        break;
        // Gaussian_sk_ model
        case Clust::Gaussian_sk_:
        { STK_CREATE_MIXTURE(MixtureDataReal, MixtureBridge_sk)}
        break;
        // Gaussian_sj_ model
        case Clust::Gaussian_sj_:
        { STK_CREATE_MIXTURE(MixtureDataReal, MixtureBridge_sj)}
        break;
        // Gaussian_s_ model
        case Clust::Gaussian_s_:
        { STK_CREATE_MIXTURE(MixtureDataReal, MixtureBridge_s)}
        break;
        // Categorical_pjk_ model
        case Clust::Categorical_pjk_:
        { STK_CREATE_MIXTURE(MixtureDataInt, MixtureBridge_pjk)}
        break;
        // Categorical_pjk_ model
        case Clust::Categorical_pk_:
        { STK_CREATE_MIXTURE(MixtureDataInt, MixtureBridge_pk)}
        break;
        default:
          return 0; // 0 if idModel is not implemented
          break;
      }
      return 0; // 0 if idModel is not a STK++ model
    }
    /** pointer to the dataHandler */
    DataHandler const* const p_handler_;
};

} // namespace STK

#undef STK_CREATE_MIXTURE

#endif /* STK_MIXTUREMANAGER_H */
