is.wholenumber <- function(x, tol = .Machine$double.eps^0.5)
{
  abs(x - round(x)) < tol
}

listModels <- c("Gaussian_sjk",
                "Categorical_pjk")

listInit <- c("randomInit",
              "randomClassInit",
              "randomFuzzyInit")

dataType <- function(modelName)
{
  if (modelName == "Gaussian_sjk") return("numeric")
  if (modelName == "Categorical_pjk") return("integer")
}