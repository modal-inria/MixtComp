is.wholenumber <- function(x, tol = .Machine$double.eps^0.5){
  abs(x - round(x)) < tol
}


listModels <- c( "Gamma_ajk_bjk"
               , "Gaussian_sjk")

listInit <- c( "randomInit"
             , "randomClassInit"
             , "randomFuzzyInit")