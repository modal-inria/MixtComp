/* Second tutorial: how to build an application using stk++
 */
namespace STK
{

/** @page TutorialApplication A stk++ application example: the MixBin program
 *
 *  @section AppIntroduction Introduction
 *
 * This tutorial will show you how to construct an application (call MixBin) using
 * the stk++ library as background. We will detail
 * @li how to read an option file,
 * @li how to read a data file and handle the missing values,
 * @li how to use an existing implemented law in stk++,
 * @li how to implement stk++ interfaces in order to estimate a statistical model.
 *
 * @section AppPresentation A (toy) statistical model
 *
 * We consider a statistical model of Bernoulli random variable of the
 * following form
 * \f[
 *     f(\mathbf{x}|\theta) =
 *      \prod_{j=1}^p p_{j}^{x^j} (1-p_{j})^{1-x^j},
 *      \quad x^j\in\{0,1\}.
 * \f]
 *
 * The model will be estimated using a sample \f$ (\mathbf{x}_i)_{i=1}^n \f$
 * with the possibility that some values are missing at random.
 *
 * We will illustrate the usage of our toy application using the
 * <em> 1984 United States Congressional Voting Records Database </em>
 * @verbatim
 Schlimmer, J. C. (1987). Concept acquisition through representational adjustment.
 Doctoral dissertation, Department of Information and Computer Science,
 University of California, Irvine, CA.
 * @endverbatim
 * that can be found at the
 * <a href="http://archive.ics.uci.edu/ml/datasets/Congressional+Voting+Records">UCI Machine Learning Repository</a>.
 *
 * The first lines of the file "house-votes-84.data" are given hereafter
 * @verbatim
   republican,n,y,n,y,y,y,n,n,n,y,?,y,y,y,n,y
   republican,n,y,n,y,y,y,n,n,n,n,n,y,y,y,n,?
   democrat,?,y,y,?,y,y,n,n,n,n,y,n,y,y,n,n
   democrat,n,y,y,n,?,y,n,n,n,n,y,n,y,n,n,y
   democrat,y,y,y,n,y,y,n,n,n,n,y,?,y,y,y,y
   ...
   @endverbatim
 * We will not make use of the first column. This data set is interesting in
 * the sense that we will have to parameterize the Read/Write classes in order
 * to adapt our reading to the coding convention used in it.
 *
 *  @section AppReadOptions Reading an option file
 *
 *  Our first task, is to communicate with the end-user. MixBin is a
 *  console-based program and the computations are driven by
 *  an option file furnished by the user like this one
 *
 * <table class="example">
 * <tr><th>%Option file "house-votes-84.opt"</th></tr>
 * <tr><td>
 * @verbinclude "../../tutorials/MixBin/data/house-votes-84.opt"
 * </td></tr>
 * </table>
 * This file can be found in the "tutorial/MixBin/data/" directory.
 *
 * In this option file, there exists only one page ([MixBin]), but there could
 * more that one pages: for example we could have added a page for the outputs,
 * a specific page for the parameters of the models, and so on...
 * @sa IPage, @sa ReadWritePages.
 *
 * @subsection AppOptionClass How to implement a new Page
 *
 * The DManager project allows to extend the pre-defined class IPage in order to
 * read options file like this one. For each page, we have to
 * implement the @c IPage interface class
 * <table class="example">
 * <tr><th>MixBinPage class header</th></tr>
 * <tr><td>
 * @include "../../tutorials/MixBin/src/MixBinPage.h"
 * </td></tr>
 * </table>
 * The constructor have been implemented in the following way
 * @code
 * MixBinPage::MixBinPage() : IPage(_T("MixBin"), 1, false)
 *                          , fileName_()
 *                          , mapping_()
 *                          , dims_(2,1) // dims_=2:2
 * {
 *   mapping_.insert(std::pair<Binary, String>(zero_, String(_T("0"))));
 *   mapping_.insert(std::pair<Binary, String>(one_, String(_T("2"))));
 *   mapping_.insert(std::pair<Binary, String>(one_, String(_T("2"))));
 *
 *   options_.reserve(3);
 *   options_.push_back(Option(_T("filename"), Option::string_, false));
 *   options_.push_back(Option(_T("mapping"), Option::lstring_, true));
 *   options_.push_back(Option(_T("dims"), Option::range_, true));
 *   options_.back().setValue(typeToString(dims_));
 * }
 * @endcode
 * we give the name of the page in the constructor of IPage and initialize
 * the members of the class to its default value. Observe that some options are
 * optional. The only one necessary is the filename option. The second option,
 * "mapping", is a @c  Option::lstring_, it means that when we will read this
 * option we will get a list of String.
 *
 * The final step is to overwrite the default value in the map @c mapping_ if
 * the option mapping is present. This can be achieved by overwriting the
 * @c validate() virtual function. This function is called when a page is read
 * from a file and allows to check if the values given are correct. Here is the
 * piece of code that accomplish this task
 * @code
 * bool MixBinPage::validate()
 * {
 *   std::list<String> const& coding = options_[1].get(std::list<String>());
 *   // ... get the other fields, more tests, here
 *   typename std::list<String>::const_iterator it = coding.begin();
 *   if (coding.size()>0) mapping_[zero_] = *it; ++it;
 *   if (coding.size()>1) mapping_[one_]  = *it; ++it;
 *   if (coding.size()>2) mapping_[binaryNA_] = *it;
 * }
 * @endcode
 *
 * @subsection AppReadWritePageClass Reading option pages
 *
 * When the pages required by the application have been defined, they can be
 * read using the class @c ReadWritePages. In the MixBin application, we use it
 * directly in the main.
 * @code
 *  ReadWritePages pages;
 *  pages.addPage(MixBinPage());
 *  if (!pages.read(option_file)) { ... } // manage any error
 *  pages.write(stk_cout);                // write the options in the console
 *  MixBinPage const* p_page = static_cast<MixBinPage const*>(pages[0]); // get the first page, MixBinPage
 *  std::string fileName = p_page->fileName();
 *  std::map<STK::Binary, STK::String> encoding = p_page->mapping();
 *  Range dims = p_page->dims();
 * @endcode
 *
 * @section AppReadDataSet Reading a data set (csv files)
 *
 * The stk++ library provide a templated class @c TReadWriteCsv which allow to
 * read and write csv files of any type. In our example, we want to read the
 * (Binary) data set "houses-votes-84.data" which is composed of binary and
 * missing data.
 *
 * This can be achieved in the following way
 * @code
 *  TReadWriteCsv<Binary> rw(fileName);
 *  rw.setWithNames(false);
 *  rw.setInMapping(decoding);
 *  rw.setWithMapping(true);
 *  if (!rw.read()) { ...} // manage error
 * @endcode
 *
 * The map decoding have been constructed using the map encoding and
 * is just the reversed mapping. For more details have a look directly to the
 * mixbin.cpp file.
 *
 *The first few lines of the values stored in the variable @c rw are given hereafter
 * @verbatim
  .,1,0,1,0,0,0,1,1,1,0,.,0,0,0,1,0
  .,1,0,1,0,0,0,1,1,1,1,1,0,0,0,1,.
  .,.,0,0,.,0,0,1,1,1,1,0,1,0,0,1,1
  .,1,0,0,1,.,0,1,1,1,1,0,1,0,1,1,0
  .,0,0,0,1,0,0,1,1,1,1,0,.,0,0,0,0
  ...
  @endverbatim
 *
 * @note In case the data file handle different kind of data, the class
 * TReadWriteCsv can be used using String as templated parameter and it can
 * make use of the ImportFromCsv class. @sa ImportFromCsv, ExportToCsv.
 *
 * Finally we transfer the data stored in the TReadWriteCsv, into some container
 * adapted to the computation. We have mainly two kind of arrays that can be used:
 * the Array2D or the CArray classes (see @ref TutorialIntroduction). As we
 * don't need to perform algebraic operations with the data set, we chose to use
 * the Array2D class which is more flexible for managing data. Moreover, in this
 * case, the transfer from the TReadWriteCsv to the Array2D is linear in the
 * number of variable (and not in the number of data as if it had been with the
 * CArray class).
 *
 * The following lines of code will do the job:
 * @code
 *  CsvToArray<Array2D<Binary> > trans(rw, 0.2);
 *  if (!trans.run())  {...  } // manage any error during the transfer
 *  Array2D<Binary> data;
 *  data.move(*trans.p_data());
 * @endcode
 * Note that how we make use of the move function which allow us to transfer the
 * data in constant time (see @ref IntroThirdExemple).
 *
 * The threshold 0.2 is the rate of missing values in <em> each column </em>
 * that we can afford. If the rate of missing values is higher, the column is
 * discarded. Set 0 if you don't want NA values in your sample. But take care
 * that you may want to remove rows (samples) from your data set rather than
 * columns.
 *
 * The first lines of the value stored in @c data are given hereafter:
 * @verbatim
  1 0 1 0 0 0 1 1 1 0 . 0 0 0 1
  1 0 1 0 0 0 1 1 1 1 1 0 0 0 1
  . 0 0 . 0 0 1 1 1 1 0 1 0 0 1
  1 0 0 1 . 0 1 1 1 1 0 1 0 1 1
  0 0 0 1 0 0 1 1 1 1 0 . 0 0 0
  ...
  @endverbatim
 *
 * @subsection AppStatisticalClass How to define a Statistical Model in stk++
 *
 * Having read the data, we have now to define the statistical model. A
 * (parametric) statistical model is a collection of distributions, each of which is
 * indexed by a unique finite-dimensional parameter:
 * \f[\mathcal{P}=\{\mathbb{P}_{\theta} : \theta \in \Theta\}\f]
 * on some space \f$ \mathcal{X} \f$ and a finite sample of realization
 * of this probability law \f$ (\mathbf{x}_1,\ldots,\mathbf{x}_n) \f$.
 * @sa IMultiStatModel.
 *
 * @subsubsection AppLawClass How to define a Probability law in stk++
 *
 * The stk++ library provide some predefined tools for handling Bernoulli
 * random variates:
 * @li the @ref Law::Bernoulli class which implement the interface @ref Law::IUnivLaw
 * @li the @ref Law::JointBernoulli class which have been implemented for the purpose
 * of this tutorial and subclass the Law::JointProbability.
 *
 * @sa Law::Bernoulli, Law::JointBernoulli, Law::IUnivLaw, Law::JointProbability.
 *
 * @note A joint Bernoulli law is the joint law of independent random Bernoulli
 * variates not necessarily identically distributed.
 *
 * All these classes allow to compute the probability density function (pdf),
 * the log-pdf (lpdf) and to simulate Bernoulli random variates or (uncorrelated)
 * multi-dimensional Bernoulli random variates. The Bernoulli class implement
 * the computation of the pdf and log-pdf in the following way:
 * @code
 * Real Bernoulli::pdf(Binary const& x) const
 * {
 *   switch (x)
 *   {
 *     case zero_: return 1.-prob_;
 *     case one_:  return prob_;
 *     default: break;
 *   }
 *   return Arithmetic<Real>::NA();
 * }
 * Real Bernoulli::lpdf(Binary const& x) const
 * {
 *   switch (x)
 *   {
 *     case zero_: return (prob_ == 1) ? -Arithmetic<Real>::infinity() : std::log(1.-prob_);
 *     case one_: return (prob_ == 0) ? -Arithmetic<Real>::infinity() : std::log(prob_);
 *     default: break;
 *   }
 *   return Arithmetic<Real>::NA();
 * }
 * @endcode
 * As can be noticed, the NA (Not-Available) values are handle by the class, but
 * the returned value @c Arithmetic<Real>::NA() can be a problem. The
 * computation of the log-pdf in the JointProbability class is implemented
 * in the following way:
 * @code
 *   virtual Real lpdf( RowVector const& x) const
 *   {
 *     if (x.range() != jointLaw_.range())
 *     {STKRUNTIME_ERROR_NO_ARG(JointProbability::lpdf(x),dimensions mismatch);}
 *     Real sum = 0.;
 *     for (int j = x.firstIdx(); j <= x.lastIdx(); ++j)
 *     { sum+= Arithmetic<Type>::isNA(x[j]) ? 0. : jointLaw_[j].lpdf(x[j]);}
 *     return sum;
 *   }
 * @endcode
 *
 * @subsubsection AppModelClass How to define a statistical model in stk++
 *
 * Let us see how to build a statistical model in stk++. For the MixBin
 * application we have two ways to define the binary model. We can either
 * implement the @c IUnivStatModel interface and create a statistical model for
 * each column of the array @c data or implement the @c IMultiStatModel interface
 * class.
 *
 * These interfaces are running classes, derived from the interface
 * @ref IRunnerConst. A IRunnerConst declare two pure virtual functions
 * @c run() and @c run(weights). The call to these methods will estimate the
 * parameters of the model.
 *
 * In the first case, the code will be the following
 * @code
 *  BernoulliModel< Array2DVector<Binary> > univariateModel;
 *  Array2DVector<Binary> var;
 *  for (int j= data.firstIdxCols(); j <= data.lastIdxCols(); ++j)
 *  {
 *    stk_cout << "j=" << j << "\n";
 *    var.move(data.col(j));
 *    univariateModel.setData(var);
 *    univariateModel.run();
 *    stk_cout << "uModel.law().prob() = " << univariateModel.law().prob() << "\n";
 *  }
 * @endcode
 * and in the second case the code will be
 * @code
 *  JointBernoulliModel<Array2D<Binary> > jb(data);
 *  jb.run();
 *  for (int j= data.firstIdxCols(); j <= data.lastIdxCols(); ++j)
 *  {
 *    stk_cout << "j=" << j << "\n";
 *    stk_cout << "jb.law().prob(j) = " << jb.law().prob(j) << "\n";
 *  }
 * @endcode
 **/

}
