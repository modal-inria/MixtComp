/*
 * tuto_StatModel.h
 *
 *  Created on: 23 oct. 2013
 *      Author: iovleff
 */

/* Contents of the tutorials
 **/

/** @page TutorialStatModel Interfaces for the Statistical Models and Probabilities Laws
 *
 *  @section StatModelIntroduction The IMultiStatModel interface
 *
 *  All the multivariate statistical models should inherit from the
 *  templated interface class
 *  @code
 *  template <class Array, class Parameters> class IMultiStatModel;
 *  @endcode
 *  The first template parameter Array represent the type of a two dimensional
 *  Array deriving from ITContainer. It should be an Array2D or a
 *  CArray container. The second template parameter is a convenient class
 *  that will store and manage the parameters of the statistical model
 *  and derive from the interface IParameters.
 *
 *  The Array have to provide two typedef @c Row and @c Col and two
 *  accessors @c row and @c col which can be used in the following form
 *  @code
 *    Array data;
 *    data.col(j); // return a typename Array::Col column vector
 *    data.row(i); // return a typename Array::Row row vector
 *  @endcode
 *
 *  The class IMultiStatModel inherit from:
 *  <ol>
 *  <li> template < class Array, class WColVector> class IRunnerUnsupervised,
 *  hich ask for implementing the following virtual functions
 *  @code
 *    bool run() =0;
 *    bool run(WColVector const& weights) =0;
 *    void update() {};
 *  @endcode
 *  The first @c run() method is used when the user of the class want to estimate
 *  the parameters of the model by, for example, maximizing the log-likelihood.
 *  The second @c run(weights) methods is used when the user of the class want
 *  to estimate the parameters of the model by maximizing the weighted log-likelihood.
 *  @c update() is to be overloaded if when assigning a data set there is some
 *  tasks to process.
 *
 *  @note The two @c run methods are implemented by @c IMultiStatModel.
 *  </li>
 *
 *  <li> IModelBase, which furnish the following accessors
 *  @code
 *    int const& nbSample() const;
 *    Real lnNbSample() const;
 *    int const& nbVariable() const;
 *    Real lnLikelihood() const;
 *    Real likelihood() const;
 *    int const& nbFreeParameter();
 *  @endcode
 *  The number of samples and variables are determined by the size of the Array
 *  while the other fields have to be computed and updated by derived classes.
 *  </li>
 * </ol>
 *
 * The @c run() (and @c run(weights)) virtual functions are implemented by
 * the @c IMultiStatModel class in the following way:
 * @code
 *    // compute (weighted) parameters
 *    computeParameters(); // or computeParameters(weights);
 *    // compute log-likelihood
 *    this->setLnLikelihood(computeLnLikelihood());
 *    // set the number of free parameters
 *    this->setNbFreeParameter(computeNbFreeParameters());
 * @endcode
 * and derived implementations of this interface have to implement the
 * following pure methods:
 *  @code
 *    int computeNbFreeParameters() const;
 *    Real computeLnLikelihood( RowVector const& rowData) const = 0;
 *    virtual void computeParameters() = 0;
 *    virtual void computeParameters(WColVector const& weights) = 0;
 *  @endcode
 *
 *  @section StatModelLaw The Law::IMultilaw interface
 *
 *  The class Law::IMultilaw is a templated interface class
 *  @code
 *  template<class RowVector> class IMultiLaw;
 *  @endcode
 *  The typedef @c RowVector must be the same than the typedef @c Array::Row.
 *
 *  The class IMultLaw derive from the interface class ILawBase and have to
 *  implement the following pure virtual methods
 *  @code
 *    virtual Real pdf( RowVector const& x) const =0;
 *    virtual Real lpdf( RowVector const& x) const =0;
 *    virtual void rand( RowVector& x) const =0;
 *  @endcode
 *  It is a very general Interface that can be used when data are correlated.
 *
 *  @subsection StatModelJointLaw The Law::JointProbability class
 *  The Law::JointProbability is derived from IMultiLaw and is defined as
 *  @code
 *    template<class RowVector, class Law> class JointProbability;
 *  @endcode
 *  the template class RowVector is described in @ref StatModelIntroduction.
 *  The template class Law, is expecting a class derived from the interface
 *  class Law::IUnivLaw.
 *
 *  For example the Law::JointBernoulli class is defined as
 *  @code
 *  template<class RowVector>
 *   class JointBernoulli: public JointProbability<RowVector, Bernoulli>
 *  @endcode
 *  and allow to modelize a Binary data set when assuming that
 *  each column is a sample of an independent Bernoulli law.
 *  @sa Law::JointBernoulli
 *
 **/

