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
 * For example the diagonal Gaussian mixture model @c STK::Gaussian_sjk is
 * the most general diagonal Gaussian model and have a density function of the form
 * \f[
 *  f(\mathbf{x}|\theta) = \sum_{k=1}^K p_k \prod_{j=1}^d
 *    \frac{1}{\sqrt{2\pi}\sigma^j_{k}} \exp\left\{-\frac{1}{2}\left(\frac{x^j-\mu^j_k}{\sigma^j_{k}}\right)^2\right\}
 * \f]
 * All the parameters are cluster specific. There is no shared parameters
 * and \f$ \boldsymbol{\lambda}_k = \{ (\mu^j_k)_{j=1}^d, (\sigma^j_{k})_{j=1}^d \} \f$
 *
 * On the other side the diagonal Gaussian mixture model @c STK::Gaussian_s is
 * the most parsimonious diagonal Gaussian model and have a density function of
 * the form
 * \f[
 *  f(\mathbf{x}|\theta) = \sum_{k=1}^K p_k \prod_{j=1}^d
 *    \frac{1}{\sqrt{2\pi}\sigma} \exp\left\{-\frac{1}{2}\left(\frac{x^j-\mu^j_k}{\sigma}\right)^2\right\}.
 * \f]
 * In both cases the means are class specific parameters, i.e.
 * \f$ \boldsymbol{\lambda}_k = \{ (\mu^j_k)_{j=1}^d \}\f$
 * while in the second model the common standard deviation is a shared parameter, i.e.
 * \f$ \boldsymbol{\alpha} = \{ \sigma \} \f$.
 *
 * We will illustrate this tutorial with these two examples.
 *
 * @section ClusterImplementation Classes and static functions to implement
 *
 * When implementing a mixture model, there is imperatively three classes
 * (the Parameter, the Component and the %Model) to implement. In a model, it is
 * required to implement the following methods:
 * @code
 *  void initializeStep();
 *  void randomInit();
 *  void mStep();
 *  Real impute(int i, int j) const;
 *  Real sample(int i, int j) const;
 *  Real safeValue(int j) const;
 *  void getParameters(Array2D<Real>& params) const;
 *  void writeParameters(ostream& os) const;
 *  int computeNbFreeParameters() const;
 * @endcode
 * and if there is shared parameters, the following method:
 * @code
 *  void initializeModel();
 * @endcode
 * These methods will be called by the STK::MixtureBridge class.
 *
 * @sa STK_MixtureBridge.h
 *
 * @section ParameterClass Implementing the Parameter class
 *
 * The first class to implement is a @c Parameter class deriving (recursively)
 * from @ref STK::IMultiParameters. This class will be created with each component
 * and must store the class specific parameters and have a pointer on the shared
 * parameters of the mixture.
 *
 * It is required by @c STK::IMultiParameters that the derived class @c Parameter
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
 * @subsection GaussianParameterBase Gaussian Examples: Common base class for the parameters
 *
 * In the Gaussian case, all the model have class specific means. We define thus
 * a base class storing the mean:
 * @code
 * template<class Parameters>
 * class DiagGaussianParametersBase : public IMultiParameters<Parameters>
 * {
 *   ...
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
 * @subsection GaussianFullParameter Gaussian Example: Parameters of the Gaussian_sjk model
 *
 * The base class @c DiagGaussianParametersBase is derived by the final class
 * @c Gaussian_sjk_Parameters in this way
 * @code
 * class Gaussian_sjk_Parameters: public DiagGaussianParametersBase<Gaussian_sjk_Parameters>
 * {
 *   ...
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
 * @sa STK_DiagGaussianParameters.h
 *
 * @subsection GaussianParcimoniousParameter Gaussian Example: Parameters of the Gaussian_s model
 *
 * The base class @c DiagGaussianParametersBase is derived by the final class
 * @c Gaussian_s_Parameters in this way
 * @code
 * class Gaussian_s_Parameters: public DiagGaussianParametersBase<Gaussian_s_Parameters>
 * {
 *   ...
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
 * @section ComponentClass Implementing the Component class
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
 *   ...
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
 * @note the component does not need to know the underlying model. It uses only the
 * base class methods @c mean(j) and @c sigma(j).
 *
 * @sa STK_DiagGaussianComponent.h
 *
 *
 * @section ModelClass Implementing the Model class
 *
 * The third mandatory class to implement is the Model class itself. It have to
 * derive (recursively) from the @ref STK::IMixtureModel class.
 *
 * At an intermediary step, we create the base class STK::DiagGaussianBase
 * for all the diagonal model
 * @code
 * template<class Derived>
 * class DiagGaussianBase : public IMixtureModel<Derived >
 * {...}
 * @endcode
 * and implement the methods
 * @code
 *  Real impute(int i, int j) const;
 *  Real sample(int i, int j) const;
 *  Real safeValue(int j) const;
 *  void getParameters(Array2D<Real>& params) const;
 *  void writeParameters(ostream& os) const;
 * @endcode
 *
 * @sa STK_DiagGaussianBase.h
 *
 * @subsection GaussianFullModel Gaussian Example: The Gaussian_sjk model
 *
 * The model STK::Gaussian_sjk have no shared parameters. It is thus the
 * simplest to implement. The main methods to implement are
 * @code
 * template<class Array>
 * class Gaussian_sjk : public DiagGaussianBase<Gaussian_sjk<Array> >
 * {
 *   ...
 *   void initializeStep();
 *   void randomInit();
 *   void mStep();
 *   inline int computeNbFreeParameters() const
 *   { return 2*this->nbCluster()*this->nbVariable();}
 *
 * }
 * @endcode
 *
 * @subsection GaussianParcimoniousModel Gaussian Example: The Gaussian_s model
 *
 * The model STK::Gaussian_s have the shared parameter sigma. Thus we have to
 * sur-define the method @c initializeModel
 *
 * @code
 * template<class Array>
 * class Gaussian_s : public DiagGaussianBase<Gaussian_s<Array> >
 * {
 *    ...
 *   virtual void initializeModel()
 *   {
 *     Base::initializeModel();
 *     for (int k= components().firstIdx(); k <= components().lastIdx(); ++k)
 *     { components()[k]->p_param()->p_sigma_ = &sigma_;}
 *   }
 *   void initializeStep();
 *   void randomInit();
 *   void mStep();
 *   inline int computeNbFreeParameters() const
 *   { return this->nbCluster()*this->nbVariable() + 1;}
 *     ...
 *   protected:
 *     Real sigma_;
 * };
 * @endcode
 *
 * @warning When there is shared parameters in the model, you have to share them
 * between the parameters components.
 *
 * @sa STK_Gaussian_s.h
 *
 * @section Integration Integrating the mixture model in the Clustering project
 *
 * It remains to integrate the mixture models to the Clustering project.
 * Currently, it is not possible to define directly a mixture model using the
 * implemented model. It is necessary to used a STK::MixtureComposer
 * which compose STK::MixtureBridge deriving from the Interface STK::IMixture.
 *
 * @sa STK_MixtureComposer.h
 *
 * In the file STK_Clust_Util.h, the enum Mixture have to be completed.
 * and the function createMixtru
 * @code
 * IMixture* createMixture(Mixture model, std::string const& id, int nbCluster)
 * @endcode
 * have to be modified accordingly.
 *
 * @sa STK_Clust_Util.h
 *
 * In the directory Mixtures, the STK::Clust::MixtureTraits class have to be
 * specialized.
 *
 * @sa STK_GaussianMixtures.h
 *
 **/
