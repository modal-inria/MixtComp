is.wholenumber <- function(x, tol = .Machine$double.eps^0.5){
  abs(x - round(x)) < tol
}


listModels <- c( "Gamma_ajk_bjk"
               , "Gamma_ajk_bj"
               , "Gaussian_sjk"
               , "Gaussian_s")

listInit <- c( "randomInit"
             , "randomClassInit"
             , "randomFuzzyInit")