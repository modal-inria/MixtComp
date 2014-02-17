/* Arrays tutorial: how to modify arrays
 **/

/** @page  ArraysConstructors Arrays Tutorial 2 : The Constructors
 *
 * Constructors depends of the structure of the array you want to obtain. In
 * this tutorial we will review all the existing arrays and the different way
 * to create them. We are
 *
 * @section AllArray2DConstructors Constructors for the Array2D family containers
 *
 * @subsection AllArray2DClasses Review of the classes
 *
 * <table border >
 * <tr> <th> class </th> <th> Description </th> </tr>
 *
 * <tr>
 * <td>
 * @code
 * template<class Type> Array2D;
 * @endcode
 * </td>
 * <td>
 * The @ref STK::Array2D class is the most general class for storing scalars.
 * The values are strored in an array of range
 * (firstIdxRows:lastIdxRows)x(firstIdxCols:lastIdxCols).
 * </td>
 * </tr>
 *
 * <tr>
 * <td>
 * @code
 * template<class Type> Array2DVector;
 * template<class Type> Array2DPoint;
 * @endcode
 * </td>
 * <td>
 * The @ref STK::Array2DVector and @ref STK::Array2DVector classes allow to store scalars
 * in respectively a column and a row vector.
 * The values are stored in an array of range (firstIdx:lastIdx).
 * </td>
 * </tr>
 *
 * <tr>
 * <td>
 * @code
 * template<class Type> Array2DSquare;
 * @endcode
 * </td>
 * <td>
 * The @ref STK::Array2DSquare class is a general class for storing scalars in
 * a square matrix. The values are stored in an array of range
 * (firstIdx:lastIdx)x(firstIdx:lastIdx).
 * </td>
 * </tr>
 *
 * <tr>
 * <td>
 * @code
 * template<class Type> Array2DDiagonal;
 * @endcode
 * </td>
 * <td>
 * The @ref STK::Array2DDiagonal class is a general class for storing scalars in
 * a diagonal matrix. The values are stored in an array of range
 * (firstIdx:lastIdx)x(firstIdx:lastIdx) with zero outside the diagonal.
 * @note Only the diagonal values are effectively stored in an Array2DDiagonal
 * array.
 * </td>
 * </tr>
 *
 * <tr>
 * <td>
 * @code
 * template<class Type> Array2DUpperTriangular;
 * template<class Type> Array2DLowerTriangular;
 * @endcode
 * </td>
 * <td>
 * The @ref STK::Array2DUpperTriangular and  STK::Array2DLowerTriangular classes are
 * general classes for storing scalars in triangular matrices.
 * The values are stored in an array of range
 * (firstIdxRows:lastIdxRows)x(firstIdxCols:lastIdxCols) with zero in the
 * respectively lower and upper part of the array.
 * @note Only the non-zeros values are effectively stored in the arrays and
 * the upper part (respectively the lower part) is the part of the array such
 * that j>i (resp. i>j), where i represents the index of the row and j the index
 * of the column.
 * </td>
 * </tr>
 *
 * </table>
 *
 * @subsection Array2DConstructors Review of the constructors
 *
 * <table border >
 * <tr> <th>Constructors </th> <th> Examples </th> </tr>
 * <tr>
 * <td>
 * Default constructors. Create an empty Array of scalar @c Type.
 * @code
 * Array2D<Type>();
 * Array2DVector<Type>();
 * Array2DPoint<Type>();
 * Array2DSquare<Type>();
 * Array2DDiagonal<Type>();
 * Array2DLowerTriangular<Type>();
 * Array2DUpperTriangular<Type>();
 * @endcode
 * </td>
 * <td>
 * In the following example
 * @code
 * Array2D<Real> a; // a is an empty array of Real (double by default)
 * a.resize(10,10); a=0.; // a is now an array of size 10x10 with the value 0.
 * Array2DVector<Real> b; // a is an empty column vector of Real
 * b.resize(10); b=0.; // b is now an array of size 10x1 with the value 0.
 * @endcode
 * </td>
 * </tr>
 *
 * <tr>
 * <td>
 * Construct an array with fixed ranges for the rows and the columns and
 * Optionally an initial value.
 * @code
 * Array2D<Type>( Range const& I, Range const& J);
 * Array2D<Type>( Range const& I, Range const& J, Type const& v);
 * Array2DVector<Type>( Range const& I);
 * Array2DVector<Type>( Range const& I, Type const& v);
 * Array2DPoint<Type>( Range const& J);
 * Array2DPoint<Type>( Range const& J, Type const& v);
 * Array2DSquare<Type>( Range const& I);
 * Array2DSquare<Type>( Range const& I, Type const& v);
 * Array2DDiagonal<Type>( Range const& I);
 * Array2DDiagonal<Type>( Range const& I, Type const& v);
 * Array2DLowerTriangular<Type>( Range const& I, Range const& J);
 * Array2DLowerTriangular<Type>( Range const& I, Range const& J, Type const& v);
 * Array2DUpperTriangular<Type>( Range const& I, Range const& J);
 * Array2DUpperTriangular<Type>( Range const& I, Range const& J, Type const& v);
 * @endcode
 * </td>
 * <td>
 * The following code build an array of Real (double by default) with rows in
 * the range 2:4 and columns in the range 1:10 (this is the default, but can be
 * 0:9 if the macro STKBASEARRAYS=0 is activated). The array is initialized with
 * the value 2.
 * @code
 * Array2D<Real> a(Range(2,3), 10, 2.);
 * @endcode
 * The following code build a vector of Real (double by default) with columns
 * in the range 1:10 (this is the default, but can be 0:9 if the macro STKBASEARRAYS=0
 * is activated). The array is initialized with the value 2.
 * @code
 * Array2DVector<Real> a( 10, 2.);
 * @endcode
 * </td>
 * </tr>
 *
 * <tr>
 * <td>
 * Copy constructor. The resulting array is either a copy of the array @c T (if @c ref == @c false)
 * or a reference of the array @c T  (if @c ref == @c true)
 * @code
 * Array2D<Type>( const Array2D<Type> &T, bool ref=false);
 * Array2DVector<Type>( const Array2DVector<Type> &T, bool ref=false);
 * Array2DPoint<Type>( const Array2DPoint<Type> &T, bool ref=false);
 * Array2DSquare<Type>( const Array2DSquare<Type> &T, bool ref=false);
 * Array2DDiagonal<Type>( const Array2DDiagonal<Type> &T, bool ref=false);
 * Array2DLowerTriangular<Type>( const Array2DLowerTriangular<Type> &T, bool ref=false);
 * Array2DUpperTriangular<Type>( const Array2DUpperTriangular<Type> &T, bool ref=false);
 * @endcode
 * </td>
 *
 * <td>
 * The following code copy the array @c a in a new array @c b and create a
 * reference of the rows and columns of @c c with range I for the rows and
 * range J for the columns.
 * @code
 * Array2D<Real> a(10, 8);
 * Array2D<Real> b(a); // b is now a copy of a
 * // c is a reference, if an element of c is modified, the corresponding element of a is modified
 * Array2D<Real> c(a.sub(I,J), true);
 * @endcode
 * The following show how to get a reference on a column and a reference on a row
 * of an Array2D.
 * @code
 * Array2D<Real> a(10, 8);
 * Array2DVector<Real> c(a.col(1), true); // c is now a reference on the column 1 of a
 * Array2DPoint<Real> p(a.row(1), true); // p is now a reference on the row 1 of a
 * @endcode
 * </td>
 * </tr>
 *
 * <tr>
 * <td>
 * Wrapper constructor. Wrap the data pointed by @c q in an Array with rows in
 * the range I and columns in the range J.
 * @code
 * Array2D<Type>( Type** q, Range const& I, Range const& J);
 * @endcode
 * </td>
 * <td>
 * In this example we create an array of size (10, 5) and wrap it in an Array2.
 * @code
 * Real** q = new Real*[10];
 * for (int j=0; j<10; j++) { q[j] = new Real[5];}
 * //wrap q. Note that q will not be freed when a is deleted
 * Array2D<Real> a(q, Range(0,10), Range(0,5)); //
 * @endcode
 * </td>
 * </tr>
 * </table>
 **/
