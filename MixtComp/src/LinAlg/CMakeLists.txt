set(LinAlg_src
  mixt_ConstIterator.h
  mixt_EigenMatrixBaseAddons.h
  mixt_Iterator.h
  mixt_LinAlg.h
  mixt_Math.cpp mixt_Math.h
  mixt_Typedef.h
)

add_library(LinAlg ${LinAlg_src})
