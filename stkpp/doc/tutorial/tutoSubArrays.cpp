#include "../../include/STKpp.h"
using namespace STK;
/** @ingroup tutorial */
int main(int argc, char** argv)
{
  // create a vector of Real of dynamic size (3) with coefficients initialized to 0
  Array2DVector<Real> b(3, 0);
  b.sub(Range(2)) = 1.; // set value 1 to the element (1:2)
  std::cout << "b=\n" << b;
  // create a matrix of Real of dynamic size (3,5)
  Array2D<Real> a(3, 4); a << 1.,2.,3.,4.
                            , 1.,2.,3.,4.
                            , 1.,1.,1.,1.;
  std::cout << "a=\n" << a;
  // modify column 1, row 1 in range 3:4 and sub-array (2:3, 3:4) of a
  a.col(1) = b;
  a.row(1, Range(3,4,0)) = 0.;
  a.sub(Range(2,1), Range(3,1)) = 9.;
  std::cout << "a=\n" << a;
  return 0;
}
