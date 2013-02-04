#include "../../include/STKpp.h"
using namespace STK;
/** @ingroup tutorial */
int main(int argc, char** argv)
{
  // create a square Matrix of Real of size (3)
  Array2DSquare<Real> a(4); a << 1.,2.,3.,4.
                               , 1.,2.,3.,4.
                               , 1.,1.,1.,1.
                               , 1.,1.,1.,1.;
  // create a reference
  Array2D<Real> b(a.sub(Range(2), Range(3)), true);
  b = -1.;
  std::cout << "a=\n" << a;
  // create a matrix of Real of fixed size (3,4)
  CArray<Real,Arrays::general_, 3, 4> c; c << 1.,2.,3.,4.
                                            , 1.,2.,3.,4.
                                            , 1.,1.,1.,1.;
  // create a reference
  CArray<Real, Arrays::vector_, 3, 1> d(c.col(1), true);
  d = -1.;
  std::cout << "c=\n" << c;
  // compute e = c*a
  Array2D<Real> e;
  e.move(mult(c, a));
  std::cout << "e=\n" << e;
  return 0;
}
