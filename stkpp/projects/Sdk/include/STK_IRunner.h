/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2011  Serge Iovleff

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
 * Project:  stkpp::Sdk
 * created on: 29 juil. 2011
 * Purpose:  main interface base class for running method.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_IRunner.h
 *  @brief In this file we define the Interface base class for all the
 *  running classes.
 **/

#ifndef STK_IRUNNER_H
#define STK_IRUNNER_H

#include "STKernel/include/STK_String.h"

namespace STK
{
/** @ingroup Sdk
 *  @brief Abstract base class for all classes having a
 *  @code bool run(); @endcode method.
 *  All classes deriving from this class should implement the @c run method
 *  using this kind of code
 *  @code
 *  bool MyClass::run()
 *  {
 *    try
 *    {
 *    // do something
 *
 *    }
 *    catch(const Exception& e)
 *    { msg_error_ = e.error();
 *      return false;
 *    }
 *    return true;
 *  }
 *  @endcode
 **/
class IRunnerBase
{
  protected:
    /** default constructor */
    inline IRunnerBase() : msg_error_() {}
    /** copy constructor
     * @param runner the runner to copy
     **/
    inline IRunnerBase( IRunnerBase const& runner) : msg_error_(runner.msg_error_) {}
    /** destructor*/
    inline ~IRunnerBase() {}

  public:
    /** get the last error message.
     * @return the last error message
     **/
    inline String const& error() const { return msg_error_;}
    /** run the computations.
     * @return @c true if no error occur during the running process, @c false
     * otherwise
     **/
    virtual bool run() =0;

  protected:
    /** String with the last error message. */
    String msg_error_;
};

/** @ingroup Sdk
 *  @brief Abstract class for all running class based on a data set.
 *
 *  This Interface is an extension of the IrunnerBase class for runners
 *  using a data set. The data set to use is not copied and a pointer on the
 *  data set to used is stored internally.
 *
 *  The pure virtual method to implement is inherited from IRunnerBase
 *  @code
 *    bool run();
 *  @endcode
 *  and the virtual method that can be overloaded are
 *  @code
 *    void setData( Array const* p_data);
 *    void update();
 *  @endcode
 *  The @c update() method is called when a new data set is set using the default
 *  implementation of @c setData.
 **/
template < class Array>
class IRunnerWithData : public IRunnerBase
{
  protected:
    /** default constructor. */
    inline IRunnerWithData() : p_data_(0) {}
    /** constructor with a pointer on the constant data set
     *  @param p_data pointer on the data set to run
     **/
    inline IRunnerWithData( Array const* const p_data) : p_data_(p_data) {}
    /** constructor with a constant reference on the data set
     *  @param data data set to run
     **/
    inline IRunnerWithData( Array const& data) : p_data_(&data) {}
    /** copy constructor
     *  @param runner the runner to copy
     **/
    inline IRunnerWithData( IRunnerWithData const& runner)
                          : IRunnerBase(runner)
                          , p_data_(runner.p_data_)
    {}
    /** destructor*/
    inline ~IRunnerWithData() {}

  public:
    /** get the data set
     * @return a constant reference on the data set.
     **/
    inline Array const* p_data() const { return p_data_;}
    /** Set the data set. If the state of the derived runner change when a new
     *  data set is set the user have to overload the udpate() method.
     *  @param p_data A pointer on the data set to run
     **/
    inline virtual void setData( Array const* p_data)
    {
      p_data_ = p_data;
      update();
    }
    /** Set the data set. If the state of the derived runner change when a new
     *  data set is set the user have to overload the udpate() method.
     *  @param data The data set to run
     **/
    inline virtual void setData( Array const& data)
    {
      p_data_ = &data;
      update();
    }

  protected:
    /** A pointer on the original data set. */
    Array const* p_data_;
    /** update the runner.
     *  This virtual method will be called when the state of the runner will
     *  change, i.e. when a new data set is set. By default do nothing.
     **/
    inline virtual void update() {}
};

/** @ingroup Sdk
 *  @brief Abstract class for all classes making unsupervised learning.
 *
 *  This Interface is designed for unsupervised learning purpose. In a
 *  supervised learning setting, use IRunnerRegression. The data set to
 *  process is not copied and a ptr on the data set is stored internally.
 *
 *  The pure virtual methods to implement are
 *  @code
 *    bool run();
 *    bool run(weights);
 *  @endcode
 **/
template < class Array, class WColVector>
class IRunnerUnsupervised : public IRunnerBase
{
  protected:
    /** default constructor. */
  inline IRunnerUnsupervised() : p_data_(0) {}
    /** constructor with a pointer on the constant data set
     *  @param p_data pointer on the data set to run
     **/
    inline IRunnerUnsupervised( Array const* const p_data) : p_data_(p_data) {}
    /** constructor with a constant reference on the data set
     *  @param data data set to run
     **/
    inline IRunnerUnsupervised( Array const& data) : p_data_(&data) {}
    /** copy constructor
     *  @param runner the runner to copy
     **/
    inline IRunnerUnsupervised( IRunnerUnsupervised const& runner)
                              : IRunnerBase(runner)
                              , p_data_(runner.p_data_)
    {}
    /** destructor*/
    inline ~IRunnerUnsupervised() {}

  public:
    /** get the data set
     * @return a constant reference on the data set.
     **/
    inline Array const* p_data() const { return p_data_;}
    /** Set the data set. If the state of the derived runner change when a new
     *  data set is set the user have to overload the udpate() method.
     *  @param p_data A pointer on the data set to run
     **/
    inline virtual void setData( Array const* p_data)
    {
      p_data_ = p_data;
      update();
    }
    /** Set the data set. If the state of the derived runner change when a new
     *  data set is set the user have to overload the udpate() method.
     *  @param data The data set to run
     **/
    inline virtual void setData( Array const& data)
    {
      p_data_ = &data;
      update();
    }
    /** run the computations.
     * @return @c true if no error occur during the running process, @c false
     * otherwise
     **/
    virtual bool run() =0;
    /** run the weighted computations.
     *  @param weights the weights of the samples
     *  @return @c true if no error occur during the running process, @c false
     *  otherwise
     **/
    virtual bool run( WColVector const& weights) =0;

  protected:
    /** A pointer on the original data set. */
    Array const* p_data_;
    /** update the runner.
     *  This virtual method will be called when the state of the runner will
     *  change, i.e. when a new data set is set. By default do nothing.
     **/
    inline virtual void update() {}
};

/** @ingroup Sdk
 *  @brief Abstract class for all classes making supervised learning.
 *
 *  This Interface is designed for supervised learning purpose. In an
 *  unsupervised learning setting, use IRunnerUnsupervised.
 *  The data sets x and y are not copied. There is two pointers on the data
 *  sets stored internally.
 *
 *  The pure virtual method to implement are
 *  @code
 *    bool run();
 *    bool run(weights);
 *  @endcode
 *
 **/
template < typename TY, typename TX>
class IRunnerRegression : virtual public IRunnerBase
{
  protected:
    /** default constructor
     *  @param p_x A pointer on the x data set to run
     *  @param p_y A pointer on the y data set to run
     **/
    IRunnerRegression( TY const* const& p_y, TX const* const& p_x)
                     : p_y_(p_y)
                     , p_x_(p_x)
    {}
    /** default constructor
     *  @param x The x data set to run
     *  @param y The y data set to run
     **/
    IRunnerRegression( TY const& y, TX const& x)
                     : p_y_(&y)
                     , p_x_(&x)
    {}
    /** copy constructor
     * @param runner the runner to copy
     **/
    IRunnerRegression( IRunnerRegression const& runner)
                     : IRunnerBase(runner)
                     , p_y_(runner.p_y_)
                     , p_x_(runner.p_x_)
    {}
    /** destructor*/
    inline ~IRunnerRegression() { }

  public:
    /** set the x data set (predictors). If the state of the runner change when
     *  a new x data set is set, the user of this class have to overload the
     *  udpate() method.
     *  @param x The x data set to run
     */
    virtual void setX( TX const& x)
    {
      p_x_ = &x;
      updateX();
    }

    /** set the data set. If the state of the runner change when a new data
     *  set is set, the user of this class have to overload the udpate() method.
     *  @param y The y data set to run
     */
    virtual void setY( TY const& y)
    {
      p_y_ = &y;
      updateY();
    }
    /** set the data set. If the state of the runner change when a new data
     *  set is set, the user of this class have to overload the udpate() method.
     *  @param y The y data set to run
     *  @param x The x data set to run
     */
    virtual void setData( TX const& y, TY const& x)
    {
      p_y_ = &y;
      p_x_ = &x;
      update();
    }
    /** run the computations.
     * @return @c true if no error occur during the running process, @c false
     * otherwise
     **/
    virtual bool run() =0;
    /** run the weighted computations.
     *  @param weights the weights of the samples
     *  @return @c true if no error occur during the running process, @c false
     *  otherwise
     **/
    virtual bool run( typename TY::Col const& weights) =0;

  protected:
    /** A pointer on the original data set. */
    TY const* p_y_;
    /** A pointer on the original data set. */
    TY const* p_x_;
    /** @brief update the runner when y data set is set.
     * This virtual method will be called when the state of the runner will
     * change, i.e. when a new y data is set is set. By default do nothing.
     **/
    virtual void updateY() {}
    /** @brief update the runner when x data set is set.
     *  This virtual method will be called when the state of the runner will
     *  change, i.e. when a new x data set is set. By default do nothing.
     **/
    virtual void updateX() {}
    /** update the runner.
     * This virtual method will be called when the state of the runner will
     * change, i.e. when new x and y data sets are set. By default do nothing.
     **/
    virtual void update() { updateX(); updateY();}
};

} // namespace STK

#endif /* STK_IRUNNER_H */
