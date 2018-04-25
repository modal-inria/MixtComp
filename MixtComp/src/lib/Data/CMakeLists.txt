set(Data_src
  mixt_AugmentedData.cpp mixt_AugmentedData.h
  mixt_CategoricalDataStat.cpp mixt_CategoricalDataStat.h
  mixt_ClassDataStat.cpp mixt_ClassDataStat.h
  mixt_ConfIntDataStat.h
  mixt_ZClassInd.cpp mixt_ZClassInd.h
)

add_library(Data ${Data_src})
