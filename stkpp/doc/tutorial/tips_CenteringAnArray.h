/* Arrays tutorial: how to modify arrays
 **/

/** @page  TipsArraysCentering Tips: Centering an array
 *
 * It' very easy to center a data set using high level templated expressions
 * and statistical functors.
 *
 * Consider the following code and its output:
 * <table class="example">
 * <tr><th>Example:</th><th>Output:</th></tr>
 * <tr><td>
 * @include tutoCenteringAnArray.cpp
 * </td>
 * <td>
 * @verbinclude tutoCenteringAnArray.out
 * </td></tr></table>
 *
 * the expression
 * @code
 * Const::Vector<Real>(100) * mean(A)
 * @endcode
 * represent the matrix multiplication of a column vector of 1 with 100 rows
 * and of row vector with the mean of each column of A.
 *
 * @note For each column of the array @c A we can get the maximal value in
 * absolute value using @c max(A.abs()). It is possible to use statistical
 * functors mixed with unary or binary operators.
 **/
