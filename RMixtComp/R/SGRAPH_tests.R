characterVectorTest <- function() {
  vec <- c("a", "ergdf", "sfdsfs")
  input <- list(d = vec)
#  input <- list(vec)
  output <- SGraphTest(input)
  print(identical(input, output))
  
  return(output)
}