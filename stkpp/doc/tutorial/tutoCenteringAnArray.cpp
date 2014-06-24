#include "STKpp.h"
using namespace STK;
int main(int argc, char *argv[])
{
  CArray<Real> A(100, 5);
  Law::Normal law(1,2);
  law.randArray(A);
  // call column statistical functions
  stk_cout << _T("min(A) =") << Stat::min(A);
  stk_cout << _T("max(A) =") << Stat::max(A);
  stk_cout << _T("max(A.abs()) =") << Stat::max(A.abs());
  stk_cout << _T("mean(A) =") << Stat::mean(A);
  // center the array
  stk_cout << _T("\nCentering...\n\n");
  A -= Const::Vector<Real>(100) * Stat::mean(A);
  // call column statistical functions with all the columns of A centered
  stk_cout << _T("min(A) =") << Stat::min(A);
  stk_cout << _T("max(A) =") << Stat::max(A);
  stk_cout << _T("max(A.abs()) =") << Stat::max(A.abs());
  stk_cout << _T("mean(A) =") << Stat::mean(A);
}
