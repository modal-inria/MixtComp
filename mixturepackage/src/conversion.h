#ifndef CONVERSION_H_
#define CONVERSION_H_
#include <Rcpp.h>

/**
 * @brief This templated function can be used to convert between various types of matrices
 * @param matrixinput Input matrix
 * @param matrixOutput Output Matrix
 * @param rows Number of rows
 * @param cols Number of columns
 */
template<class inp,class out>
inline void convertMatrix(const inp& matrixinput,out& matrixOutput,int rows,int cols)
{
   for(int i=0;i<rows;i++)
   {
     for(int j=0;j<cols;j++)
     {
       matrixOutput(i,j) = matrixinput(i,j);
     }
   }
}

/**
 * @brief This templated function can be used to convert between various types of vectors
 * @param vectorinput Input vector
 * @param vectoroutput Output vector
 * @param len Length of vector
 */
template<class inp,class out>
inline void convertvector(const inp& vectorinput,out& vectoroutput,int len)
{
    for (int i = 0; i < len; ++i) {
      vectoroutput(i) = vectorinput(i);
    }
}

#endif /* CONVERSION_H_ */
