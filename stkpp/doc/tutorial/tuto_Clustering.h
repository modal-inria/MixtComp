/*
 * tuto_Clustering.h
 *
 *  Created on: 25 oct. 2013
 *      Author: iovleff
 */


/** @page TutorialClustering Clustering: Develop Your Own Mixture %Model
 *
 * @section ClusterIntroduction About Mixture Models
 *
 * This tutorial will show you how to add a new mixture model to stk++.
 * The design of the Clustering model have been think with in mind less
 * effort for the programmer who need to add a mixture model.
 *
 * A mixture model on some subset \f$ J\subset \mathbb{R}^d \f$ is a density of
 * the form
 * \f[
 *     f(\mathbf{x}|\boldsymbol{\theta})
 *     = \sum_{k=1}^K p_k f(\mathbf{x};\boldsymbol{\lambda}_k,\boldsymbol{\alpha})
 *     \quad \mathbf{x} \in J.
 * \f]
 *
 * The \f$ p_k > 0\f$ with  \f$ \sum_{k=1}^K p_k =1\f$ are the mixing proportions.
 * The density \e f is called the component of the model. The parameters
 * \f$\boldsymbol{\lambda}_k, \, k=1,\ldots K \f$ are the cluster specific parameters
 * and the parameters \f$ \boldsymbol{\alpha} \f$ are the shared parameters.
 *
 * The whole set of parameters is thus
 * \f[
 * \boldsymbol{\theta} = \{ (p_k)_{k=1}^K, (\boldsymbol{\lambda}_k)_{k=1}^K, \boldsymbol{\alpha} \}.
 * \f]
 *
 * For example the diagonal Gaussian mixture model @c Gaussian_sjk is
 * the most general diagonal Gaussian model and have a density function of the form
 * \f[
 *  f(\mathbf{x}|\theta) = \sum_{k=1}^K p_k \prod_{j=1}^d
 *    \frac{1}{\sqrt{2\pi}\sigma^j_{k}} \exp\left\{-\frac{1}{2}\left(\frac{x^j-\mu^j_k}{\sigma^j_{k}}\right)^2\right\}
 * \f]
 * All the parameters are cluster specific. There is no shared parameters
 * and \f$ \boldsymbol{\lambda}_k = \{ (\mu^j_k)_{j=1}^d, (\sigma^j_{k})_{j=1}^d \} \f$
 *
 * On the other side the diagonal Gaussian mixture model @c Gaussian_s is
 * the most parsimonious diagonal Gaussian model and have a density function of
 * the form
 * \f[
 *  f(\mathbf{x}|\theta) = \sum_{k=1}^K p_k \prod_{j=1}^d
 *    \frac{1}{\sqrt{2\pi}\sigma} \exp\left\{-\frac{1}{2}\left(\frac{x^j-\mu^j_k}{\sigma}\right)^2\right\}.
 * \f]
 * In both cases the means are class specific parameters, i.e.
 * \f$ \boldsymbol{\lambda}_k = \{ (\mu^j_k)_{j=1}^d \}\f$
 * and the common standard deviation is a shared parameter, i.e.
 * \f$ \boldsymbol{\alpha} = \{ \sigma \} \f$.
 *
 * We will illustrate this tutorial with these two examples.
 *
 * @section ClusterImplementation Classes and static functions to implement
 *
 * When implementing a mixture model, there is imperatively three classes
 * (the Parameter, the Component and the %Model) to implement and at least
 * two static functions to implement in the STK::MixtureModelImpl wrapping
 * class.
 *
 * @subsection ParameterClass Implementing the Parameter class
 *
 * The first class to implement is a @c Parameter class deriving (recursively)
 * from @ref STK::IMultiParameters. This class will be created with each component
 * and must store the class specific parameters and have a pointer on the shared
 * parameters of the mixture.
 *
 * It is required by @c IMultiParameters that the derived class @c Parameter
 * implement the methods
 * @code
 *   void resizeImpl(Range const& range);
 *   void printImpl(ostream &os);
 * @endcode
 *
 * The method @c resizeImpl have to resize the containers for the shared parameters
 * in order to match the range of the variables.
 *
 * The method @c printImpl have to print the whole parameters.
 *
 * @subsubsection GaussianParameterBase Gaussian Examples: Common base class for the parameters
 *
 * In the Gaussian case, all the model have class specific means. We define thus
 * a base class storing the mean:
 * @code
 * template<class Parameters>
 * class DiagGaussianParametersBase : public IMultiParameters<Parameters>
 * {
 *   protected:
 *     inline DiagGaussianParametersBase() : mean_() {}
 *     inline DiagGaussianParametersBase( Range const& range) : mean_(range, 0.) {}
 *     inline DiagGaussianParametersBase( DiagGaussianParametersBase const& param) : mean_(param.mean_) {}
 *     inline ~DiagGaussianParametersBase() {}
 *   public:
 *     inline Real mean(int j) const {return mean_[j];}
 *     inline Real sigma(int j) const {return this->asDerived().sigmaImpl(j);}
 *     Array2DPoint<Real> mean_;
 * }
 * @endcode
 *
 * At this level, it is also required to the derived class to implement the method
 * @code
 *   void sigmaImpl(int j);
 * @endcode
 * but all models will access to the j-th mean and standard deviation using
 * @c mean(j) and @c sigma(j).
 *
 * @note The default constructor and the copy constructor are mandatory, hovewer
 * by completeness we add a constructor with the range of the variables given.
 *
 * @sa STK_DiagGaussianParameters.h,
 *
 * @subsubsection GaussianFullParameter Gaussian Example: Parameters of the Gaussian_sjk model
 *
 * The base class @c DiagGaussianParametersBase is derived by the final class
 * @c Gaussian_sjk_Parameters in this way
 * @code
 * class Gaussian_sjk_Parameters: public DiagGaussianParametersBase<Gaussian_sjk_Parameters>
 * {
 *   public:
 *     typedef DiagGaussianParametersBase<Gaussian_sjk_Parameters> Base;
 *     inline Gaussian_sjk_Parameters() : Base(), sigma_() {}
 *     inline Gaussian_sjk_Parameters( Range const& range): Base(range), sigma_(range, 1.) {}
 *     inline Gaussian_sjk_Parameters( Gaussian_sjk_Parameters const& param)
 *                                       : Base(param), sigma_(param.sigma_) {}
 *     inline ~Gaussian_sjk_Parameters() {}
 *     inline Real sigmaImpl(int j) const {return sigma_[j];}
 *     inline void resizeImpl(Range const& size)
 *     {
 *       mean_.resize(size); mean_ = 0.;
 *       sigma_.resize(size); sigma_ = 1.;
 *     }
 *     inline void printImpl(ostream &os) { os << mean_ << sigma_ << _T("\n");}
 *     Array2DPoint<Real> sigma_;
 * };
 * @endcode
 *
 *
 * @subsubsection GaussianParcimoniousParameter Gaussian Example: Parameters of the Gaussian_s model
 *
 * The base class @c DiagGaussianParametersBase is derived by the final class
 * @c Gaussian_s_Parameters in this way
 * @code
 * class Gaussian_s_Parameters: public DiagGaussianParametersBase<Gaussian_s_Parameters>
 * {
 *   public:
 *     typedef DiagGaussianParametersBase<Gaussian_sjk_Parameters> Base;
 *     inline Gaussian_s_Parameters() : Base(), p_sigma_(0) {}
 *     inline Gaussian_s_Parameters( Range const& range): Base(range), p_sigma_(0) {}
 *     inline Gaussian_s_Parameters( Gaussian_s_Parameters const& param)
 *                                     : Base(param), p_sigma_(param.p_sigma_) {}
 *     inline ~Gaussian_s_Parameters() {}
 *     inline Real sigmaImpl(int j) const {return *p_sigma_;}
 *     inline void resizeImpl(Range const& size)
 *     { mean_.resize(size); mean_ = 0.;}
 *     inline void printImpl(ostream &os)
 *     { os << mean_ << *p_sigma_ * Const::Point<Real>(mean_.range()) << _T("\n");}
 *     Real* p_sigma_;
 * };
 * @endcode
 *
 * @sa STK_DiagGaussianParameters.h
 * @note The pointer p_sigam_ is initialized to NULL (0), as the common value
 * sigma_ is stored by the model class @c Gaussian_s.
 * @warning If the pointer value is not set at the creation of the model, you
 * will experiment an unexpected behavior.
 *
 *
 * @subsection ComponentClass Implementing the Component class
 *
 * The second mandatory class to create is a Component class deriving from
 * @ref STK::IMixtureComponent. There is a Component object created for the @e K
 * components of the model. The Component have only to implement the pure virtual
 * function
 * @code
 *   virtual Real computeLnLikelihood( RowVector const& rowData) const;
 * @endcode
 * and should be independent of the way the parameters are stored. Remember that
 * a component is essentially a multidimensional density.
 *
 * For all the diagonal Gaussian models, we have a single component class
 * @code
 * template <class _Array, class _Parameters>
 * class DiagGaussianComponent : public IMixtureComponent< _Array, _Parameters >
 * {
 *   public:
 *     typedef _Array Array;
 *     typedef _Parameters Parameters;
 *     typedef typename _Array::Row RowVector;
 *     typedef IMixtureComponent< _Array, _Parameters > Base;
 *     using Base::p_param;
 *     inline DiagGaussianComponent() : Base() {}
 *     inline DiagGaussianComponent(_Array const& data) : Base(data) {}
 *     inline DiagGaussianComponent(_Array const* p_data) : Base(p_data) {}
 *     inline DiagGaussianComponent( DiagGaussianComponent const& model): Base(model) {}
 *     inline virtual ~DiagGaussianComponent() {}
 *     inline DiagGaussianComponent* clone() const { return new DiagGaussianComponent(*this);};
 *     virtual Real computeLnLikelihood( RowVector const& rowData) const
 *     {
 *       Real sum =0.;
 *       for (Integer j= rowData.firstIdx(); j <= rowData.lastIdx(); ++j)
 *       { sum += Law::Normal::lpdf(rowData[j], p_param()->mean(j), p_param()->sigma(j));}
 *       return sum;
 *     }
 * };
 * @endcode
 *
 * @subsection ModelClass Implementing the Model class
 * The third mandatory class to implement is the Model class itself. It have to
 * derive (recursively) from the @ref STK::IMixtureModel class.
 * There is no virtual function to be re-implemented by the model.
 *
 * The model have to take care of the shared parameters and to initialize the
 * parameters by setting the address of these shared parameters in the method:
 * @code
 *   void initializeModel();
 * @endcode
 *
 * @subsubsection GaussianFullModel Gaussian Example: The Gaussian_sjk model
 * This model have no shared parameters. It is thus the simplest to implement.
 * @code
 * template<class Array>
 * class Gaussian_sjk : public IMixtureModel<Gaussian_sjk<Array> >
 * {
 *   public:
 *     typedef IMixtureModel<Gaussian_sjk<Array> > Base;
 *     using Base::p_data_;
 *     using Base::components_;
 *     Gaussian_sjk( int nbCluster) : Base(nbCluster) {}
 *     Gaussian_sjk( int nbCluster, Array const& data) : Base(nbCluster, data) {}
 *     Gaussian_sjk( int nbCluster, Array const* p_data) : Base(nbCluster, p_data) {}
 *     Gaussian_sjk( Gaussian_sjk const& model) : Base(model) {}
 *     virtual ~Gaussian_sjk() {}
 *     virtual void writeParameters(ostream& os) const
 *     {
 *       stk_cout << _T("lnLikelihood = ") << this->lnLikelihood() << _T("\n";);
 *       for (int k= components_.firstIdx(); k <= components_.lastIdx(); ++k)
 *       {
 *         stk_cout << _T("---> Component ") << k << _T("\n";);
 *         stk_cout << _T("mean_ = ") << components_[k]->p_param()->mean_;
 *         stk_cout << _T("sigma_ = ") << components_[k]->p_param()->sigma_;
 *       }
 *     }
 *     inline int computeNbFreeParameters() const
 *     { return 2*this->nbCluster()*this->nbVar()+ this->nbCluster()-1;}
 *   };
 * @endcode
 *
 * @subsubsection GaussianParcimoniousModel Gaussian Example: The Gaussian_s model
 * This model have the shared parameter sigma.
 * @code
 * template<class Array>
 * class Gaussian_s : public IMixtureModel<Gaussian_s<Array> >
 * {
 *   public:
 *     typedef IMixtureModel<Gaussian_s<Array> > Base;
 *     using Base::p_data_;
 *     using Base::components_;
 *     Gaussian_s( int nbCluster) : Base(nbCluster) {}
 *     Gaussian_s( int nbCluster, Array const& data) : Base(nbCluster, data) {}
 *     Gaussian_s( int nbCluster, Array const* p_data) : Base(nbCluster, p_data) {}
 *     Gaussian_s( Gaussian_s const& model) : Base(model) {}
 *     virtual ~Gaussian_s() {}
 *     virtual void initializeModel()
 *     {
 *       scale_.resize(p_data_->cols());
 *       IMixtureComposerBase::initializeModel();
 *       for (int k= components_.firstIdx(); k <= components_.lastIdx(); ++k)
 *       { components_[k]->p_param()->p_sigma_ = &sigma_;}
 *     }
 *     virtual void writeParameters(ostream& os) const
 *     {
 *       stk_cout << _T("lnLikelihood = ") << this->lnLikelihood() << _T("\n";);
 *       for (int k= components_.firstIdx(); k <= components_.lastIdx(); ++k)
 *       {
 *         stk_cout << _T("---> Component ") << k << _T("\n";);
 *         stk_cout << _T("mean_ = ") << components_[k]->p_param()->mean_;
 *         stk_cout << _T("sigma_ = ") << sigma_ * Const::Point<Real>(this->nbVar());
 *       }
 *     }
 *     inline int computeNbFreeParameters() const
 *     { return this->nbCluster()*this->nbVar();}
 *   protected:
 *     Real sigma_;
 * };
 * @endcode
 *
 * @warning When there is shared parameters in the model, you have to share them
 * between the parameters components.
 *
 * @subsection StaticMethods Implementing the randomInit and mStep static methods
 *
 * The last step is to implement the model specific methods randomInit and mStep
 * static function in the class
 * @code
 * template<class Array, class Parameter, class Component > struct MixtureModelImpl;
 * @endcode
 * These methods form the core of the initialization and estimation algorithms
 * and depend of the mixture model. They have the following form
 * @code
 *  static void randomInit(Array1D< Component* >& components);
 *  static void mStep(Array1D< Component* >& components, Array2D<Real> const* p_tik);
 * @endcode
 * and will modify the parameters (cluster-specifics and shared) accordingly.
 *
 * @subsubsection GaussianBaseStaticMethods Gaussian Examples: Common methods for the Gaussian models
 *
 * For all the Gaussian models the mean parameters are treated in the same way.
 * We create thus an utility struct
 * @code
 * template<class Component> struct GaussianUtil;
 * @endcode
 * and implement in this structure the two static functions
 * @code
 *   static void randomMean(Array1D< Component* >& components)
 *   {
 *     if (components.size() <= 0) return;
 *     for (int k= components.firstIdx(); k <= components.lastIdx(); ++k)
 *     {
 *       Parameters* paramk = components[k]->p_param();
 *       Array const* p_data = components[k]->p_data();
 *       // random number in [0, size[
 *       int i = p_data->firstIdxRows() + std::floor(Law::Uniform::rand(0.,1.)*p_data->sizeRows());
 *       paramk->mean_.copy(p_data->row(i));
 *     }
 *   }
 *   static void updateMean(Array1D< Component* >& components, Array2D<Real> const* p_tik)
 *   {
 *     if (components.size() <= 0) return;
 *     for (int k= p_tik->firstIdxCols(); k <= p_tik->lastIdxCols(); ++k)
 *     {
 *       Parameters* paramk = components[k]->p_param();
 *       Array const* p_data = components[k]->p_data();
 *       ColVector tik(p_tik->col(k), true); // create a reference
 *       for (int j=p_data->firstIdxCols(); j<=p_data->lastIdxCols(); ++j)
 *       {
 *         Real mean =  p_data->col(j).wmeanSafe(tik);
 *         if ((mean<=0)||Arithmetic<Real>::isNA(mean)) throw Clust::estimFail_;
 *         paramk->mean_[j] = mean;
 *       }
 *     }
 *   }
 * @endcode
 * The mean are chosen randomly among the data set and are updated using the
 * weighted mean.
 *
 * @subsubsection GaussianParcimoniousStaticMethods Gaussian Example: The Gaussian_sjk model
 *
 * @code
 * static void randomInit(Array1D< Component* >& components)
 * {
 *   if (components.size() <= 0) return;
 *   GaussianUtil<Component>::randomMean(components);
 *   for (int k= components.firstIdx(); k <= components.lastIdx(); ++k)
 *   {
 *     Parameters* paramk = components[k]->p_param();
 *     Array const* p_data = components[k]->p_data();
 *     paramk->sigma_ = 1.;
 *   }
 * }
 * static void mStep(Array1D< Component* >& components, Array2D<Real> const* p_tik)
 * {
 *   if (components.size() <= 0) return;
 *   // compute the means
 *   GaussianUtil<Component>::updateMean(components, p_tik);
 *   for (int k= p_tik->firstIdxCols(); k <= p_tik->lastIdxCols(); ++k)
 *   {
 *     Parameters* paramk = components[k]->p_param();
 *     Array const* p_data = components[k]->p_data();
 *     ColVector tik(p_tik->col(k), true); // create a reference
 *     for (int j=p_data->firstIdxCols(); j<=p_data->lastIdxCols(); ++j)
 *     {
 *       Real mean =  paramk->mean_[j];
 *       Real variance = Stat::varianceWithFixedMean(p_data->col(j), tik, mean, false);
 *       if ((variance<=0)||Arithmetic<Real>::isNA(variance)) throw Clust::estimFail_;
 *       paramk->sigma_[j] = std::sqrt(variance);
 *     }
 *   }
 * }
 * @endcode
 *
 * @subsubsection GaussianParcimoniousStaticMethods Gaussian Example: The Gaussian_s model
 *
 * @code
 * static void randomInit(Array1D< Component* >& components)
 * {
 *   if (components.size() <= 0) return;
 *   GaussianUtil<Component>::randomMean(components);
 *   Parameters* paramk = components.front()->p_param();
 *   *(paramk->p_sigma_) = 1.;
 * }
 * static void mStep(Array1D< Component* >& components, Array2D<Real> const* p_tik)
 * {
 *   if (components.size() <= 0) return;
 *   // compute the means
 *   GaussianUtil<Component>::updateMean(components, p_tik);
 *   // compute the standard deviation
 *   Array const* p_data = components.front()->p_data();
 *   Real variance = 0.0;
 *   for (int k= p_tik->firstIdxCols(); k <= p_tik->lastIdxCols(); ++k)
 *   {
 *     Parameters* paramk = components[k]->p_param();
 *     variance += ( p_tik->col(k).transpose() * (*p_data - (Const::Vector<Real>(p_data->rows()) * paramk->mean_)).square()).sum();
 *   }
 *   if ((variance<=0)||Arithmetic<Real>::isNA(variance)) throw Clust::estimFail_;
 *   *(components.front()->p_param()->p_sigma_) = std::sqrt(variance/(p_data->sizeRows()*p_data->sizeCols()));
 * }
 * @endcode
 *
 * @section Integration Integrating the mixture in the Clustering project
 *
 * It remains to integrate your code to the Clustering project.
 *
 * Currently, it is not possible to define directly a mixture model using the
 * implemented model. It is necessary to used a STK::MixtureComposer
 * which compose mixtures deriving from the Interface STK::Imixture.
 *
 *
 *
 * @code
 * IMixture* createMixture(Mixture model, std::string const& id, int nbCluster)
 * @endcode
 *
 **/
