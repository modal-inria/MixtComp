set(IO_src
  Dummy.cpp Dummy.h
  mixt_DataExtractorCsv.cpp mixt_DataExtractorCsv.h
  mixt_IO.h
  mixt_MisValParser.h
  mixt_ParamExtractorCsv.cpp mixt_ParamExtractorCsv.h
  mixt_SpecialStr.cpp mixt_SpecialStr.h
  
)

add_library(IO ${IO_src})
