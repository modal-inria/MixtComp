/* Arrays tips: how to modify arrays
 **/

/** @page  TipsComputeMeanByColumn Tips: Compute the mean for each column of an array
 *
 * You can easily get the mean of a whole vector or a matrix using the expression
 * @code
 *   CArray<Real> A(100, 20);
 *   Law::Normal law(1,2);
 *   law.randArray(A);
 *   Real m = A.mean();
 * @endcode
 *
 * In some cases you may want to get the mean for each column of the array
 * with (possibly) missing values. You can obtain them using the code
 * @code
 *   Array2DPoint<Real> m;
 *   m = Stat::mean(A.safe()); // or m.move(mean(A.safe()));
 * @endcode
 *
 * The method @c A.safe() will replace any missing (or NaN) values by zero.
 * In some cases it's not sufficient, Suppose you know your data are all
 * positive and you want to compute the log-mean of your data. In this case, use
 * rather
 * @code
 *   m = Stat::mean(A.safe(1.).log());
 * @endcode
 * and all missing (or NaN) values will be replaced by one.
 *
 * @note You can also compute the variance. If you want to compute the mean of
 * each row, you will have to use the functor meanByRow. In this latter case,
 * you will get an Array2DVector as a result.
 *
 * @sa Stat::meanSafe, Stat::variance, Stat::varianceWithFixedMean
 **/
