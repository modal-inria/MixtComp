/* kennel tutorial: how to used the STK++ kernel tools
 **/

/**@page PageTutorialKernel Description of the STKernel projects: The Kernel tools.
 *
 * The STKernel project is the low-level core library that forms the basis
 * of the STK++ library. This tutorial intent to give some help to the
 * Developers who want to use the STK++ fundamental types and i-o facilities
 * provided by the library.
 *
 * In statistics, missing data, or missing values, occur when no data value
 * is stored for the variable in the current observation. Missing data are a
 * common occurrence and can have a significant effect on the conclusions that
 * can be drawn from the data. Heterogeneous Data is data from any number of
 * sources, largely unknown and unlimited, and in many varying formats. In
 * essence, it is a way to refer to data that id of an unknown format and/or
 * content.
 *
 * From a computational point of view, the programmer have to take care of these
 * two aspects. It is desirable to
 * - have some internal representation of the missing data,
 * - to be able to read/write missing values in a transparent way,
 * - to identify dynamically the type of data the program handles.
 *
 * Theses goals have been achieved in STK++ by
 * - extending the numeric_limits struct with specific static methods,
 * - designing a Runtime Type Identification (RTTI) mechanism for handling
 * heterogeneous data.
 *
 * @see Arithmetic, IdTypeImpl
 *
 * Additionally, the STKernel project provides typedef of the @c std::basic_*
 * streams in the STK namespace domain for the predefined @c Char type which can
 * be either a @c char or a @c wchar. Thus a programmer can write
 * @code
 *   STK::ifstream ifs;
 * @endcode
 * and compile the library using the flag -DSTK_UNICODE, without taking care all
 * along the code if he has to use ifstream or wifstream.
 *
 *  @section STKernelMissingData Handling the missing values
 *
 * In the version 0.5.3 of STK++ there exists 6 kinds of fundamental types: String,
 * Real, Integer, Binary, Sign and Range. All these types are self-explaining
 * except maybe Range which represents a range of integer in the form first:last.
 *
 * If we take the example of the Real type, we have
 * @code
 *  typedef  double Real;
 * @endcode
 * which can be replaced by float if the user need single precision rather
 * that double precision.
 *
 * If now we take the example of the Binary type, we have
 * @code
 *   enum Binary
 *   { zero_ =0, ///< 0 value
 *     one_  =1, ///< 1 value
 *     binaryNA_= __MAX_INT__ ///< Not Available value
 *  };
 * @endcode
 * and it is important to note that any integer type represents missing values
 * with the __MAX_INT__ macro.
 *
 * The piece of code
 * @code
 * istringstream in(stringNa);
 * Real x;
 * in >> Proxy<Real>(x);
 * Binary b = STK::Arithmetic<Binary>::NA(); // b is a Binary missing value
 * stk_cout << _T("x =") << Proxy<Real>(x) << _T("\n");
 * stk_cout << _T("b =") << Proxy<Binary>(b) << _T("\n");
 * @endcode
 * will produce the following output
 * @verbatim
     x =.
     b =.
   @endverbatim
 * The missing values are displayed using the dot (".") symbol. This is a
 * global behavior that can be modified at runtime by using the global method
 * @c setStringNa. For example using:
 * @code
 *   setStringNa(_T("NA"));
 * @endcode
 * all the input and output operation will be altered and NA value will be
 * read/write using NA.
 *
 * @note In order to produce the correct output it is necessary to use the @e Proxy class.*
 * @note The use of the Proxy class does not produce more
 * computation, nor object creation, in optimized mode.
 *
 * @section STKernelArithmetic Arithmetic properties
 *
 * The Arithmetic struct inherit from all the static functions of the
 * @c std::numeric_limits struct and add static functions for handling the NA
 * values of all STK++ type in a transparent way. For example for Real, it is
 * specialized in the following way:
 * @code
 * template<>
 * struct Arithmetic<Real> : public std::numeric_limits<Real>
 * {
 *   static const bool has_quiet_NaN = false;
 *   static Real NA() throw() { return std::numeric_limits<Real>::quiet_NaN();}
 *   static const bool hasNA = true;
 *   static bool isNA(Real const& x) throw() { return !(x==x);}
 *   static bool isInfinite(Real const& x) throw()
 *   { return ( (x < -std::numeric_limits<Real>::max())
 *            ||(x > std::numeric_limits<Real>::max())
 *            );
 *   }
 *   static bool isFinite(Real const& x) throw() { return (!isNA(x) && !isInfinite(x));}
 * };
 * @endcode
 *
 * so that the piece of code
 * @code
 * Real x = STK::Arithmetic<Real>::NA(); // x is a real missing value
 * stk_cout << _T("x =") << typeToString(x) << _T("\n");
 * stk_cout << _T("isNa(x) =") << Arithmetic<Real>::isNa(x) << _T("\n");
 * @endcode
 * will produce the following output
 * @verbatim
     x =.
     isNa(x) =1
   @endverbatim
 *
 * @note it is rather more intuitive to use the template function
 * @c typeToString than the Proxy class for output.
 * @sa stringToType for the reverse mechanism.
 *
 *  @section STKernelRTTI Run Time Type Identification
 *
 *  All fundamental STK++ types are defined by its IdType. Knowing the type of
 *  an object, it is possible to get its Id using the piece of code
 *  @code
 *  IdTypeImpl<Type>::returnType()
 *  @endcode
 *  The value returned is an IdType.
 *
 *  Knowing the Id of an object, it is possible to tune the code of your
 *  application. This RTTI mechanism is mainly used in STK++ in the Variable
 *  and DataFrame classes.
 **/
