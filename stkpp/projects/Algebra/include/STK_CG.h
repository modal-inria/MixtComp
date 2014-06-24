/*--------------------------------------------------------------------*/
/*     Copyright (C) 2013-2013  Serge Iovleff, Quentin Grimonprez

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

    Contact : S..._DOT_I..._AT_stkpp.org (see copyright for ...)
*/

/*
 * Project:  stkpp::Algebra
 * created on: 24 mai 2013
 * Author:   Quentin Grimonprez
 **/

/** @file STK_CG.h
 *  @brief In this file we define a templated version of the conjugate gradient.
 **/


#ifndef STK_CG_H
#define STK_CG_H

#include "Sdk/include/STK_Macros.h"
#include "Sdk/include/STK_IRunner.h"

#define MAXITER 100
namespace STK
{

template<class ColVector>
struct DefaultFunctor
{
    ColVector operator()() const
    { return ColVector();}
};

/** @ingroup Algebra
 *  The conjugate gradient method is an algorithm for the numerical solution of
 *  particular systems of linear equations, namely those whose matrix is
 *  symmetric and positive-definite. The conjugate gradient method is an
 *  iterative method, so it can be applied to sparse systems that are too large
 *  to be handled by direct methods such as the Cholesky decomposition.
 *
 *  The algorithm is detailed below for solving \f$ \mathbf{A}\mathbf{x} = \mathbf{b} \f$
 *  where @b A is a real, symmetric, positive-definite matrix.
 *  The input vector @b x can be an approximate initial solution or 0.
 *
 *  \f[
 *   \begin{array}{l}
 *   \mathbf{r}_0 := \mathbf{b} - \mathbf{A x}_0  \\
 *   \mathbf{p}_0 := \mathbf{r}_0 \\
 *   k  := 0 \\
 *  \hbox{repeat} \\
 * \qquad \alpha_k  := \frac{\mathbf{r}_k^\mathrm{T} \mathbf{r}_k}{\mathbf{p}_k^\mathrm{T} \mathbf{A p}_k}  \, \\
 * \qquad \mathbf{x}_{k+1} := \mathbf{x}_k + \alpha_k \mathbf{p}_k \\
 * \qquad \mathbf{r}_{k+1}  := \mathbf{r}_k - \alpha_k \mathbf{A p}_k \\
 * \qquad \hbox{if } r_{k+1} \hbox{ is sufficiently small then exit loop} \\
 * \qquad \beta_k  := \frac{\mathbf{r}_{k+1}^\mathrm{T} \mathbf{r}_{k+1}}{\mathbf{r}_k^\mathrm{T} \mathbf{r}_k}  \\
 * \qquad \mathbf{p}_{k+1} := \mathbf{r}_{k+1} + \beta_k \mathbf{p}_k  \\
 * \qquad k  := k + 1  \\
 * \hbox{end repeat}
 * \end{array}
 * \f]
 * The result is \f$ x_{k+1}\f$
 * </p>
 *
 * @tparam MultFunctor A functor computing the result of @b Ax.
 * @tparam InitFunctor A functor computing the initial value @b x.
 * @tparam ColVector The type of the containers for the vectors.
 */
template<class MultFunctor, class ColVector, class InitFunctor = DefaultFunctor<ColVector> >
class CG : public IRunnerBase
{
  public:
    typedef typename ColVector::Type Type;
    /**Constructor
     * @param p_mult functor which compute @b Ax with @b A a matrix and @b x a vector
     * @param b from @b Ax=b
     * @param p_init functor which initialize @b x
     * @param eps tolerance
     */
    CG( MultFunctor const& mult, ColVector const& b, InitFunctor* const& p_init =0, Type eps=Arithmetic<Type>::epsilon())
      : x_()
      , r_()
      , eps_(eps)
      , p_mult_(&mult)
      , p_init_(p_init)
      , p_b_(&b)
    {};

    /**Copy constructor. The constructor to copy.
     * @param cg the conjugate gradient to copy
     */
    CG( CG const& cg)
      : x_(cg.x_)
      , r_(cg.r_)
      , eps_(cg.eps_)
      , p_mult_(cg.p_mult_)
      , p_init_(cg.p_init_)
      , p_b_(cg.p_b_)
    {};
    /**destructor*/
    virtual ~CG() {};
    /** clone pattern */
    CG* clone() const { return new CG(*this);}

    /**@return the solution of the linear system Ax=b*/
    inline ColVector const& x() const { return x_;}
    /**@return the ith coordinate of the solution of the linear system Ax=b*/
    inline Real const& x(int const& i) const { return x_[i];}
    /**@return the residuals b-A*x*/
    inline ColVector const& r() const { return r_;}
    /** Set the tolerance*/
    inline void setEps(Type const& eps) {eps_ = eps;}
    /** Set the constant vector */
    inline void setB(ColVector const& b) { p_b_=&b;}
    /** Set functor computing Ax */
    inline void setMultFunctor(MultFunctor const& mult) { p_mult_= &mult; }
    /** Set functor computing @b x at initialization */
    inline void setInitFunctor(InitFunctor* const& p_init) { p_init_=p_init; }
    /** run the conjugate gradient */
    virtual bool run()
    {
      try
      {
        cg();
      }
      catch (Exception const& e)
      {
        this->msg_error_ = e.error();
        return false;
      }
      return true;
    }

  protected:
    void cg()
    {
      int nbStart = 0;
      ColVector xOld, z, p_;

      Real bnorm2 = p_b_->norm2(), alpha, beta; //
      int step = 0; //number of step
      // initialization
      if(!p_init_) {x_ = *p_b_;}
      else { x_ = (*p_init_)();}
      while(nbStart<2)
      {
        if (bnorm2 == 0.) bnorm2 = 1.;
        //compute the residuals
        r_ = *p_b_ - (*p_mult_)(x_);
        if (r_.norm2()/bnorm2 <eps_) { break;}
        //initialization of the conjugate direction
        p_= r_;
        while(1)
        {
          Real rnorm2 = r_.norm2();
          //compute z=A p
          z.move((*p_mult_)(p_));
          //compute alpha
          alpha = rnorm2/p_.dot(z);
          //update x_
          xOld.exchange(x_);
          x_ = xOld + alpha * p_;
          //update residuals
          r_ = r_ - (alpha * z);
          //compute beta
          beta = 1/rnorm2;
          if ((rnorm2=r_.norm2())/bnorm2 <eps_) { nbStart = 2; break;}
          beta *= rnorm2;
          //update p_
          p_ = (p_ * beta) + r_;
          step++;
          if( step > MAXITER )
            throw runtime_error("CG reaches MAXITER before convergence.");
        }
        nbStart++;
      };
    }

  private:
    /** solution of the system */
    ColVector x_;
    /** residuals of the system */
    ColVector r_;
    /** tolerance */
    Type eps_;
    /** pointer on the functor performing @b Ax */
    MultFunctor const*  p_mult_;
    /** pointer on the functor initializing @b x*/
    InitFunctor const*  p_init_;
    /** constant pointer on the second member of the system */
    ColVector const* p_b_;
};

/** @ingroup Algebra
 *  In most cases, preconditioning is necessary to ensure fast convergence of
 *  the conjugate gradient method. The preconditioned conjugate gradient method
 *  takes the following form:
 *
 *  \f[
 *   \begin{array}{l}
 *   \mathbf{r}_0 := \mathbf{b} - \mathbf{A x}_0  \\
 *   \mathbf{z}_0 := \mathbf{M}^{-1} \mathbf{r}_0\\
 *   \mathbf{p}_0 := \mathbf{z}_0 \\
 *   k  := 0 \\
 *  \hbox{repeat} \\
 * \qquad \alpha_k := \frac{\mathbf{r}_k^\mathrm{T} \mathbf{z}_k}{\mathbf{p}_k^\mathrm{T} \mathbf{A p}_k} \\
 * \qquad \mathbf{x}_{k+1} := \mathbf{x}_k + \alpha_k \mathbf{p}_k \\
 * \qquad \mathbf{r}_{k+1} := \mathbf{r}_k - \alpha_k \mathbf{A p}_k \\
 * \qquad \hbox{if } r_{k+1} \hbox{ is sufficiently small then exit loop} \\
 * \qquad \mathbf{z}_{k+1} := \mathbf{M}^{-1} \mathbf{r}_{k+1} \\
 * \qquad \beta_k := \frac{\mathbf{z}_{k+1}^\mathrm{T} \mathbf{r}_{k+1}}{\mathbf{r}_k^\mathrm{T} \mathbf{r}_k} \\
 * \qquad \mathbf{p}_{k+1} := \mathbf{z}_{k+1} + \beta_k \mathbf{p}_k  \\
 * \qquad k  := k + 1  \\
 * \hbox{end repeat}
 * \end{array}
 * \f]
 * The result is \f$ x_{k+1}\f$
 * </p>
 * The above formulation is equivalent to applying the conjugate gradient method
 * without preconditioning to the system
 * \f$\mathbf{E}^{-1}\mathbf{A}(\mathbf{E}^{-1})^\mathrm{T}\mathbf{\hat{x}}=\mathbf{E}^{-1}\mathbf{b}\f$
 * where \f$\mathbf{EE}^\mathrm{T}=\mathbf{M}\f$ and
 * \f$\mathbf{\hat{x}}=\mathbf{E}^\mathrm{T}\mathbf{x}\f$.
 *
 * The preconditioner matrix M has to be symmetric positive-definite and fixed,
 * i.e., cannot change from iteration to iteration. If any of these assumptions
 * on the preconditioner is violated, the behavior of the preconditioned
 * conjugate gradient method may become unpredictable.
 *
 * @tparam MultFunctor A functor computing the result of @b Ax.
 * @tparam CondFunctor A functor computing the value \f$ \mathbf{M}^{-1} \mathbf{r}\f$.
 * @tparam InitFunctor A functor computing the initial value @b x.
 * @tparam ColVector The type of the containers for the vectors.
 */
template<class MultFunctor, class CondFunctor, class ColVector, class InitFunctor = DefaultFunctor<ColVector> >
class PCG : public IRunnerBase
{
  public:
    typedef typename ColVector::Type Type;
    /**Constructor
     * @param p_mult functor which compute @b Ax with @b A a matrix and @b x a vector
     * @param b from @b Ax=b
     * @param p_init functor which initialize @b x a vector
     * @param eps tolerance
     */
    PCG( MultFunctor const& mult, CondFunctor const& cond, ColVector const& b, InitFunctor* const& p_init =0, Type eps=Arithmetic<Type>::epsilon())
       : x_()
       , r_()
       , eps_(eps)
       , p_mult_(&mult)
       , p_cond_(&cond)
       , p_init_(p_init)
       , p_b_(&b)
    {};
    /**Copy constructor. The constructor to copy.
     * @param pcg the preconditioned conjugate gradient to copy
     */
    PCG( PCG const& pcg)
      : x_(pcg.x_)
      , r_(pcg.r_)
      , eps_(pcg.eps_)
      , p_mult_(pcg.p_mult_)
      , p_cond_(pcg.p_cond_)
      , p_init_(pcg.p_init_)
      , p_b_(pcg.p_b_)
    {};
    /**destructor*/
    virtual ~PCG() {};
    /** clone pattern */
    PCG* clone() const { return new PCG(*this);}

    /**@return the solution of the linear system Ax=b*/
    inline ColVector const& x() const { return x_;}
    /**@return the ith coordinate of the solution of the linear system Ax=b*/
    inline Real const& x(int const& i) const { return x_[i];}
    /**@return the residuals b-A*x*/
    inline ColVector const& r() const { return r_;}
    /** Set the tolerance*/
    inline void setEps(Type const& eps) {eps_ = eps;}
    /** Set the constant vector */
    inline void setB(ColVector const& b) { p_b_=&b;}
    /** Set functor computing @b x at initialization */
    inline void setInitFunctor(InitFunctor const& init) { p_init_= &init; }
    /** Set functor computing Ax */
    inline void setMultFunctor(MultFunctor const& mult) { p_mult_= &mult; }
    /** Set functor computing the value \f$ \mathbf{M}^{-1} \mathbf{r}\f$ */
    inline void setCondFunctor(CondFunctor const& cond) { p_cond_= &cond; }
    /** run the conjugate gradient */
    virtual bool run()
    {
      try
      {
        pcg();
      }
      catch (Exception const& e)
      {
        this->msg_error_ = e.error();
        return false;
      }
      return true;
    }

  protected:
    void pcg()
    {
      int nbStart = 0;
      ColVector xOld, y, z, p;

      Real bnorm2 = p_b_->norm2(), alpha, beta; //
      int step = 0; //number of step
      // initialization
      if(!p_init_) {x_ = *p_b_;}
      else { x_ = (*p_init_)();}
      // first loop -> allow to restart algorithm in case of divergence.
      while(nbStart<2)
      {
        if (bnorm2 == 0.) bnorm2 = 1.;
        //compute the residuals
        r_ = *p_b_ - (*p_mult_)(x_);
        if (r_.norm2()/bnorm2 <eps_) { break;}
        //initialization of the conjugate direction
        y = (*p_cond_)(r_);
        p = y;
        Real rty=r_.dot(y);
        while(1)
        {
          Real rnorm2 = r_.norm2();
          //compute z=A p
          z.move((*p_mult_)(p));
          //compute alpha
          alpha = rty/p.dot(z);
          //update x_
          xOld.exchange(x_);
          x_ = xOld + alpha * p;
          //update residuals
          r_ = r_ - (alpha * z);
          //update y
          y = (*p_cond_)(r_);
          //compute beta
          beta = 1/rty;
          if ((rnorm2=r_.norm2())/bnorm2 <eps_) { nbStart = 2; break;}
          rty = r_.dot(y);
          beta *= rty;
          //update p_
          p = (p * beta) + y;
          step++;
          if( step > MAXITER )
            throw runtime_error("CG reaches MAXITER before convergence.");
        }
        nbStart++;
      };
    }

  private:
    /** solution of the system */
    ColVector x_;
    /** residuals of the system */
    ColVector r_;
    /** tolerance */
    Type eps_;
    /** pointer on the functor performing @b Ax */
    MultFunctor const*  p_mult_;
    /** pointer on the functor performing \f$ \mathbf{M}^{-1} \mathbf{r}\f$*/
    CondFunctor const*  p_cond_;
    /** pointer on the functor initializing @b x*/
    InitFunctor const*  p_init_;
    /** constant pointer on the second member of the system */
    ColVector const* p_b_;
};

} // namespace STK

#undef MAXITER

#endif /* STK_CG_H_ */
