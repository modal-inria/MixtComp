# @author Vincent Kubicki
vandermonde <- function(vec, nCoeff) {
  v <- matrix(nrow = nCoeff, ncol = nCoeff)
  for (i in 1:nCoeff) {
    for (j in 1:nCoeff) {
      v[i, j] <- vec[i] ** (j - 1)
    }
  }
  return(v)
}

# @author Vincent Kubicki
evalFunc <- function(a, x) {
  nObs <- length(x)
  nCoeff <- length(a)
  y <- vector(mode = "numeric", length = length(x))

  for (i in 1:nObs) {
    y[i] <- 0.
    for (k in 1:nCoeff) {
      y[i] <- y[i] + a[k] * (x[i] ** (k - 1))
    }
  }

  return(y)
}

# @author Vincent Kubicki
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

# @author Vincent Kubicki
generate_data <- function(size = 2000,
                          ratio_test_training = 0.4,
                          path_dir_output = "",
                          correlation = TRUE, oldMC = TRUE) {
  param <- list(
    x = c(0., 10., 20., 100),
    y = c(1., 10., 1., 50),
    sd = 0.1,
    tMin = 0.,
    tMax = 20.,
    nTime = 100
  )

  data_func = c()
  y_base = param$y
  for (i in 1:size) {
    param$y = sample(1:10, size = 1) * y_base
    data_func <-
      c(data_func, functionalInterPolyGenerator(param = param))
  }

  data_gaussian = rnorm(n = size,
                               mean = c(0, 5, 10, 100),
                               sd = 2)

  data_categorical = sample(1:8, size = size, replace = TRUE)
  if (correlation) {
    data_categorical = sort(data_categorical)
  }

  data <- cbind(data_func, data_gaussian, data_categorical)
  colnames(data) = c("F1", "X1", "C1")
  descriptor <-
    setNames(data.frame(matrix(
      ncol = ncol(data),
      nrow = 2,
      byrow = TRUE
    )), colnames(data))

  if(oldMC)
    descriptor[1, ] <- c("Functional", "Gaussian_sjk", "Categorical_pjk")
  else
    descriptor[1, ] <- c("Func_CS", "Gaussian", "Multinomial")

  descriptor[2, 1] <- rep("nSub: 5, nCoeff: 2", 1)

  # test/training management
  if ((ratio_test_training < 0) | ((ratio_test_training > 1)))
  {
    stop("ratio_test_training must be between 0 and 1")
  }

  data_training = data

  if (ratio_test_training > 0) {
    data_test = data[1:(ratio_test_training * size),]
    write.table(
      data_test,
      paste0(path_dir_output, "/generated_data_test.csv"),
      sep = ";",
      na = "",
      row.names = FALSE
    )
    data_training = data[((ratio_test_training * size) + 1):size,]
  }

  write.table(
    data_training,
    paste0(path_dir_output, "/generated_data_training.csv") ,
    sep = ";",
    na = "",
    row.names = FALSE
  )

  write.table(
    descriptor,
    file = paste0(path_dir_output, "/descriptor.csv"),
    sep = ";",
    na = "",
    row.names = FALSE
  )
}
