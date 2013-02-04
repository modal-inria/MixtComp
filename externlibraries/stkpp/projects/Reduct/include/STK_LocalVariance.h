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

    Contact : Serge.Iovleff@stkpp.org
*/

/*
 * Project:  stkpp::AAModels
 * Purpose:  Implementation of the ILinearReduct interface using the local variance.
 * Author:   iovleff, serge.iovleff@stkpp.org
 **/

/** @file STK_LocalVariance.h In this file we define the LocalVariance class.
 *  @brief A LocalVariance is an implementation of the Interface class Index.
 **/

#ifndef STK_LOCALVARIANCE_H
#define STK_LOCALVARIANCE_H

#include "STK_ILinearReduct.h"

#include "../../Arrays/include/STK_Array2DSquare.h"
#include "../../STatistiK/include/STK_Stat_MultivariateMatrix.h"

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
 */
class LocalVariance : public ILinearReduct
{
  public:
    /**
     * Type of proximity graph to used in order to compute the local variance:
     * - prim_ the minimal spanning tree
     * - minimalDistance_ the first neighbors
     * - unknown_ unknown type of graph
     */
    enum TypeGraph { prim_, minimalDistance_, unknown_ };

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
    static String TypeGraphToString( TypeGraph const& type);

    /** Constructor. the TypeGraph and the number of neighbors are
     *  given by the user and are not modified.
     *  @param data the data set to process
     *  @param type type of proximity graph to build
     *  @param nbNeighbor number of neighbors to use in the proximity graph
     */
    LocalVariance( Matrix const& data
                 , TypeGraph const& type = minimalDistance_
                 , int const& nbNeighbor =1
                 );

    /** Destructor */
    virtual ~LocalVariance();

    /**
     * get the number of neighbors used n the local covariance.
     * @return an array with the index of the neighbors of an individual
     */
    inline int const& nbNeighbor() const { return nbNeighbor_;}
    /**
     * get the predeccesor array
     * @return an array with the index of the neighbors of an individual
     */
    inline Array2D<int> const& pred() const { return neighbors_;}
    /**
     * get the covariance
     * @return the covariance matrix of the data set
     */
    inline MatrixSquare const& covariance() const { return covariance_;}
    /**
     * get the local covariance
     * @return the local covariance matrix computed using the proximity
     * graph.
     */
    inline MatrixSquare const& localCovariance() const { return *p_localCov_;}

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
    Matrix dist_;

    /** the local covariance Matrix */
    MatrixSquare * p_localCov_;
    /** the covariance Matrix */
    MatrixSquare covariance_;
    /** the multivariate Statistics. This structure is used in order to
     * compute the global covariance matrix of the data set.
     **/
    Stat::MultivariateMatrix* p_dataStatistics_;

    /**
     * Compute the axis by maximizing the ratio of the local variance on the
     * total variance of the data set.
     */
    virtual void maximizeCriteria();

    /**
     * Compute the axis by maximizing the ratio of the weighted local variance
     * on the weighted total variance of the data set.
     * @param weights the weights to use
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
    void initializeMemory();
    /** clear allocated memory */
    void clear();
    /** Default constructor. The TypeGraph and the number of neighbors are
     *  given by the user and are not modified.
     *  @param type type of proximity graph to build
     *  @param nbNeighbor number of neighbors to use in the proximity graph
     */
    LocalVariance( TypeGraph const& type = minimalDistance_
                 , int const& nbNeighbor =1
                 );
};

} // namespace STK
#endif /* STK_LOCALVARIANCE_H */
