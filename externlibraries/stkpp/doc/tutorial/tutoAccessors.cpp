#include "../../include/STKpp.h"
using namespace STK;
/** @ingroup tutorial */
int main(int argc, char** argv)
{
  // create a matrix of Real of dynamic size (3,5)
  Array2D<Real> a(3, 4); a << 1.,2.,3.,4.
                            , 1.,2.,3.,4.
                            , 1.,1.,1.,1.;
  std::cout << "a=\n" << a;
  // create a vector of Real of dynamic size (3) with all coefficients equal to 0
  Array2DVector<Real> b(3, 0);
  b[2] = 1.;
  std::cout << "b=\n" << b;
  // create a CArray of Real of fixed size (3,4) with unitialized coefficients.
  CArray<Real, Arrays::general_, 3, 4> c;
  // create a CArray of Real of dynamic size (3,4) with initial value -2.
  CArray<Real, Arrays::general_> d(3, 4, -2);
  // compute c = -a - d + 3 (Id)
  c= -a - d  + 3.;  c(2,2) = 5.;
  std::cout << "c=\n" << c;
  // create a CVector of Real of fixed size (3) with initialized coefficients.
  CArray<Real, Arrays::vector_, 3, 1> e;
  e = -2.; e[1] = 5.;
  std::cout << "e=\n" << e;
  return 0;
}
