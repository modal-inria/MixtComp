#include "../../include/STKpp.h"
using namespace STK;
using namespace STK::Stat;
int main(int argc, char *argv[])
{
  CArray<Real> A(100, 5);
  Law::Normal law(1,2);
  law.randArray(A);
  // call column statistical functions
  stk_cout << _T("min(A) =") << min(A);
  stk_cout << _T("max(A) =") << max(A);
  stk_cout << _T("max(A.abs()) =") << max(A.abs());
  stk_cout << _T("mean(A) =") << mean(A);
  // center the array
  stk_cout << _T("\nCentering...\n\n");
  A -= Const::Vector<Real>(100) * mean(A);
  // call column statistical functions with all the columns of A centered
  stk_cout << _T("min(A) =") << min(A);
  stk_cout << _T("max(A) =") << max(A);
  stk_cout << _T("max(A.abs()) =") << max(A.abs());
  stk_cout << _T("mean(A) =") << mean(A);
}
