testDF <- function(){
  height <- c(1.,2.,3.,4.,6.)
  age <- c(23.,12.,89.,65.,12.)
  
  testDF <- data.frame(age,height)
  show(tracemem(testDF$age[1]))
  
  updateDFByRef(testDF)
  
  show(testDF)
}