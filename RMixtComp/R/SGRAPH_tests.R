characterVectorTest <- function() {
  vec <- c("a", "ergdf", "sfdsfs")
  input <- list(d = vec)
  output <- SGraphTest(input)
  print(identical(input, output))
}