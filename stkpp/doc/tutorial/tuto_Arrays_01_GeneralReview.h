/* kennel tutorial: how to used the STK++ kernel tools
 **/

/** @page  ArraysFirstExemple Arrays Tutorial 1 : General Review of the STK++ Arrays and theirs usages
 *
 * The containers/arrays you use in order to store and process the data in your
 * application greatly influence the speed and the memory usage of your application.
 * STK++ propose a large choice of containers/arrays and methods that you can used in
 * conjunction with them.
 *
 * @p There are mainly two kinds of arrays you can use with STK++:
 * @li The %Array2D classes which are the classes defined in the oldest versions of STK++,
 * @li the %CArray class which have been introduced in version 0.4 of STK++ library.
 *
 * Before explaining the usage and differences between the different arrays,
 * we first introduce some vocabulary.
 * The terminology used in STK++ project for the arrays are the following:
 * @li An array is often called a matrix.
 * @li In the case where a matrix have 1 column, such matrix is called column-vector,
 * often abbreviated just as @em vector,
 * @li in the other case, where a matrix have 1 row, it is called row-vector, often
 * abbreviated just as @em point.
 *
 * The word @em point is borrowed from the statistical vocabulary where a row
 * of a data array is often named a point.
 *
 * The %Array2D classes are very flexible if you need to add, insert, remove, resize,...
 * quickly rows or columns to your container. On the other hand, the storing
 * scheme of the the %CArray classes allow you to used them easily with other
 * linear algebra libraries (e.g. Lapack, Blas, ...).
 *
 * @section IntroExample Introductory Example
 * Let us give you an introductory example:
 * <table class="example">
 * <tr><th>Example:</th><th>Output:</th></tr>
 * <tr><td>
 * @include tutoAccessors.cpp
 * </td>
 * <td>
 * @verbinclude tutoAccessors.out
 * </td></tr></table>
 *
 * @section IntroAccessors Accessors
 * The primary coefficient accessors and mutators in STK++ are the overloaded
 * parenthesis operators. For matrices, the row index is always
 * passed first.

 * The operator[] is also overloaded for index-based access in vectors, but keep
 * in mind that C++ doesn't allow operator[] to take more than one argument. The
 * operator[] is thus restricted to vectors/points.
 * For vectors, just pass one index in a bracket.
 *
 * The numbering starts at 1. This behavior can be modify by defining the
 * STKBASEARRAYS macro at compile time using the directive -DSTKBASEARRAYS=0
 * for 0 based arrays. If you want to build code independent of the numbering
 * start you should use the @c firstIdxCols(), firstIdxRows(), lastIdxCols(),
 * lastIdxRows() method of the arrays and the @c firstIdx(), lastIdx() methods
 * of the Row-vectors, Column-Vectors, square matrices and diagonal matrices.
 *
 * For example
 * @code
 * Array2D t(5, 5); // array of size 5x5
 * for (int i=t.firstIdxRows(); i<=lastIdxRows(); i++)
 * {
 *   Array2DPoint r(t.row(i), true); // create a reference on the i-th row of t
 *   // fill the i-th row of t with the number i. Same (but slower) than r=i;
 *   for (int j=r.firstIdx(); j<=lastIdx(); j++) { r[j] = i;}
 * }
 * @endcode
 *
 * @section IntroExpTemplate Templated Expression
 *
 * Assume that @c c, a,d are array of the same size and consider the line of
 * code
 * @code
 * c= -a - d  + 3.;
 * @endcode
 * It is an expression which involve matrix operations. All these expressions
 * are encoded in an expression template and are completely inlined at compile
 * time. That mean there is no temporary objects created when these expressions
 * are evaluated.
 *
 *
 * @section IntroConstructors The constructors
 *
 * The Array2D class have only one mandatory template parameter: the type of
 * the data that will be stored.
 *
 * On the other hand the CArray class have five template parameters:
 * - the type of the data that will be stored,
 * - the Structure of the array,
 * - the number of rows if it is known at compile time,
 * - the number of columns if it is known at compile time,
 * - the Orientation scheme of the data (by row or by column).
 *
 * Except for points and vectors, only the first two parameters are mandatory.
 *
 * <table class="example">
 * <tr><th>Example:</th></tr>
 * <tr><td>
 * @include tutoConstructors.cpp
 * </td></tr></table>
 *
 * For @ref STK::Real data set, there exists pre-defined typedef in
 * the file @ref STK_CArray.h.
 *
 * @section IntroSecondExemple Accessing rows/columns/parts of an array
 *
 * You can access rows, columns and sub-part of STK++ arrays easily. Here is
 * an example:
 *
 * <table class="example">
 * <tr><th>Example:</th><th>Output:</th></tr>
 * <tr><td>
 * @include tutoSubArrays.cpp
 * </td>
 * <td>
 * @verbinclude tutoSubArrays.out
 * </td></tr></table>
 *
 * Applied to a vector/point/diagonal matrix the method sub require only
 * one parameter: the Range of the data we want to access/mutate.
 *
 * In the general case, you can use the following method in order to access/mutate
 * columns/rows/part of an array:
 * @code
 * // access to the ith row
 *  a.row(i);
 * // access to the element [first, first+size-1] of the ith row
 *  a.row(i, Range(first, size));
 * // access to the jth column
 *  a.col(j);
 * // access to the element [first, first+size-1] of the jth columns
 *  a.col(j, Range(first, size));
 * // access to the element [first, first+size-1] of the jth columns
 *  a.col(j, Range(first, size));
 * // access to the sub-array formed by the range I,J.
 *  a.sub(I, J);
 *  @endcode
 *
 *  For the %Array2D classes (deriving form the IArray2DBase class), the operator
 *  @c () have been overloaded and you can also access to row/column/sub-part
 *  of the Array using
 *  @code
 *  Range I(3,2), J(4,2);
 *  // access to the row i, in the range J = 4:5
 *  a(i,J);
 *  // access to the column j in the Range I=3:4
 *  a(I,j);
 *  // access to a subarray in the range (3:4, 4:5)
 *  a(I,J);
 *  @endcode
 *
 * @section IntroThirdExemple Using references and move
 *
 * In some cases, you may want to conserve an access to some part of an array
 * for some work. For this purpose, it is possible to create @a reference
 * array, that is array that wrap (part of) another array.
 *
 * <table class="example">
 * <tr><th>Example:</th><th>Output:</th></tr>
 * <tr><td>
 * @include tutoReference.cpp
 * </td>
 * <td>
 * @verbinclude tutoReference.out
 * </td></tr></table>
 *
 * The mult function return an %Array2D @e by @e value. In order to avoid
 * a useless copy, you have to use the move function. The following piece of
 * code
 * @code
 * a.move(b);
 * @endcode
 * perform the operations:
 * @li if @c a contains data, the memory is released,
 * @li @c a become the owner of the data contain by @c b,
 * @li @c b become a reference.
 * If @c b was a reference, then @c a is also a reference.
 *
 * Many more examples can be found in the test files:
 * @li @sa testArray2D.cpp
 * @li @sa testCArrays.cpp
 *
 **/
