is.wholenumber <- function(x, tol = .Machine$double.eps^0.5)
{
  abs(x - round(x)) < tol
}

listModels <- c("Gaussian_sjk",
                "Categorical_pjk",
                "Poisson_k")

listInit <- c("randomInit",
              "randomClassInit",
              "randomFuzzyInit")
