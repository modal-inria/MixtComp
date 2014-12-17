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
 * Project:  stkpp::AAModels
 * Purpose:  Implementation of the ILinearReduct interface using the local variance.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_LocalVariance.h In this file we define the LocalVariance class.
 *  @brief A LocalVariance is an implementation of the Interface class Index.
 **/

#ifndef STK_LOCALVARIANCE_H
#define STK_LOCALVARIANCE_H

#include "STK_ILinearReduct.h"

#include "Arrays/include/STK_Array2DSquare.h"
#include "STatistiK/include/STK_Stat_MultivariateReal.h"

namespace STK
{

/** @ingroup Reduct
 *  @brief A LocalVariance is an implementation of the abstract
 *  @c ILinearReduct class.
 *
 *  A LocalVariance is an Index which maximize the projected local
 *  variance of the data set.
 *  The class can use the algorithm of Prim or the minimal distance
 *  in order to compute the proximity graph defining the local variance.
 *
 *  This class derive from ILinearReduct which derive itself from IRunnerUnsupervised.
 *  The @c run() and @c run(weights) methods have been implemented in the
 *  ILinearReduct interface using the pure virtual methods @c maximizeCriteria()
 *  and @c MaximizeCriteria(weights).
 */
class LocalVariance : public ILinearReduct
{
  public:
    /** Type of proximity graph to used in order to compute the local variance:
     * - prim_ the minimal spanning tree
     * - distance_ the first neighbors
     * - unknown_ unknown type of graph
     */
    enum TypeGraph { prim_, distance_, unknown_ };
    /** convert a String to a TypeGraph.
     *  @param type the type of graph in a string
     *  @return the TypeGraph represented by the String @c type. If the string
     *  does not match any known name, the @c unknown_ type is returned.
     **/
    static TypeGraph stringToTypeGraph( String const& type);
    /** convert a TypeGraph to a String.
     *  @param type the type of graph we want to convert to a string
     *  @return the string associated to this type of graph
     **/
    static String typeGraphToString( TypeGraph const& type);
    /** Constructor. the TypeGraph and the number of neighbors are
     *  given by the user and are not modified.
     *  @param p_data the data set to process
     *  @param type type of proximity graph to build
     *  @param nbNeighbor number of neighbors to use in the proximity graph
     */
    LocalVariance( ArrayXX const* p_data, TypeGraph type = distance_, int nbNeighbor =1);
    /** Constructor. the TypeGraph and the number of neighbors are
     *  given by the user and are not modified.
     *  @param data the data set to process
     *  @param type type of proximity graph to build
     *  @param nbNeighbor number of neighbors to use in the proximity graph
     */
    LocalVariance( ArrayXX const& data, TypeGraph type = distance_, int nbNeighbor =1);
    /** copy Constructor.
     *  @param reductor the reductor to copy
     */
    LocalVariance( LocalVariance const& reductor);

    /** clone pattern */
    inline virtual LocalVariance* clone() const { return new LocalVariance(*this);}
    /** Destructor */
    virtual ~LocalVariance();
    /** @return the number of neighbors used in the local covariance.*/
    inline int nbNeighbor() const { return nbNeighbor_;}
    /**@return an array with the index of the neighbors of an individual */
    inline Array2D<int> const& pred() const { return neighbors_;}
    /** @return the covariance matrix of the data set */
    inline ArraySquareX const& covariance() const { return covariance_;}
    /**@return the local covariance matrix computed using the proximity graph. */
    inline ArraySquareX const& localCovariance() const { return localCovariance_;}

  protected:
    /**
     * Compute the proximity graph if the data set is modified.
     */
    virtual void update();

    /** number of neighbors */
    TypeGraph type_;
    /** number of neighbors */
    int nbNeighbor_;
    /** Predecessor array : store the spanning tree or the minimal distance
     * to the neighbors */
    Array2D<int> neighbors_;
    /** distance matrix : store the minimal distance to the neighbors */
    ArrayXX dist_;

    /** the local covariance Array */
    ArraySquareX localCovariance_;
    /** the covariance Array */
    ArraySquareX covariance_;
    /** the multivariate Statistics. This structure is used in order to
     *  compute the global covariance matrix of the data set.
     **/
    Stat::MultivariateArrayXX* p_dataStatistics_;

    /** Compute the axis by maximizing the ratio of the local variance on the
     *  total variance of the data set.
     */
    virtual void maximizeCriteria();

    /** Compute the axis by maximizing the ratio of the weighted local variance
     *  on the weighted total variance of the data set.
     *  @param weights the weights to use
     */
    virtual void maximizeCriteria( Vector const& weights);

    /** compute the minimal spanning tree */
    void prim();
    /** compute the minimal distance graph */
    void minimalDistance();

    /** compute the covariances matrices of the data set */
    void computeCovarianceMatrices();
    /** compute the weighted covariances matrices of the data set */
    void computeCovarianceMatrices( Vector const& weights);

  private:
    /** compute the axis using the first eigenvectors of the matrix
     * of covariance and local covariance
     **/
    void computeAxis();

    /** initialize memory */
    void initialize();
    /** clear allocated memory */
    void clear();

    /** Default constructor. The TypeGraph and the number of neighbors are
     *  given by the user and are not modified.
     *  @param type type of proximity graph to build
     *  @param nbNeighbor number of neighbors to use in the proximity graph
     */
    LocalVariance( TypeGraph type = distance_, int nbNeighbor =1);
};

} // namespace STK
#endif /* STK_LOCALVARIANCE_H */
