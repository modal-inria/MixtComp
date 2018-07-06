
functionalInterPolyGenerator <- function(present, param) {
  timeObs <- vector("character", param$nTime)

  nCoeff <- length(param$x)
  v <- vandermonde(param$x, nCoeff)
  a <- solve(v, param$y)
  t <- vector(mode = "numeric", length = param$nTime)

  for (i in 1:param$nTime) {
    t[i] <- runif(1, param$tMin, param$tMax)
  }
  t <- sort(t)

  for (i in 1:param$nTime) {
    x <- evalFunc(a, t[i]) + rnorm(1, mean = 0, sd = param$sd)
    timeObs[i] <- paste(t[i], x, sep = ":")
  }

  xStr <- paste(timeObs, collapse = ",")

  return(xStr)
}


#' Generate data for testing
#'
#' @param size integer, number of observatios generated
#'
#' @return void
#'
#' @examples
generate_data <- function(size=2000,ratio_test_training=0.4) {
  param <- list(
    x = c(0., 10., 20.),
    y = c(1., 10., 1.),
    sd = 0.1,
    tMin = 0.,
    tMax = 20.,
    nTime = 100
  )

  data_func = c()
  y_base = param$y
  for (i in 1:size) {
    param$y= sample(1:10,size = 1)*y_base
    data_func <- c(data_func, functionalInterPolyGenerator(param = param))
  }

  data_gaussian = rnorm(
    n = size,
    mean = c(0, 5, 10, 100),
    sd = c(1, 2, 3, 4)
  )

  data_categorical = sample(1:8,size = size,replace=T)

    data <- cbind(data_func,data_gaussian,data_categorical)
  colnames(data) = c("F1", "X1", "C1")
  descriptor <-
    setNames(data.frame(matrix(
      ncol = ncol(data),
      nrow = 2,
      byrow = TRUE
    )), colnames(data))
  descriptor[1, ] <-
    c("Functional", "Gaussian_sjk", "Categorical_pjk")
  descriptor[2, 1] <- rep("nSub: 5, nCoeff: 2", 1)

  data_test = data[1:(ratio_test_training*size),]
  data_training = data[((ratio_test_training*size)+1):size,]

  write.table(
    data_training,
    "data/generated_data_training.csv" ,
    sep = ";",
    na = "",
    row.names = F
  )
  write.table(
    data_test,
    "data/generated_data_test.csv" ,
    sep = ";",
    na = "",
    row.names = F
  )
  write.table(
    descriptor,
    file = "data/descriptor.csv",
    sep = ";",
    na = "",
    row.names = F
  )
}
