

param <- list(
  x = c(0., 10., 20.),
  y = c(0., 10., 0.),
  sd = 0.1,
  tMin = 0.,
  tMax = 20.,
  nTime = 100)

data = c()
for(i in 1:2000){
  data <- c(data,functionalInterPolyGenerator(param=param))
}

data <- cbind(data,rnorm(n = 2000,mean = c(0,5,10,100),sd=c(1,2,3,4)))

descriptor <-
  setNames(data.frame(matrix(
    ncol = ncol(data),
    nrow = 2,
    byrow = TRUE
  )), colnames(data))
descriptor[1,] <-
  c("Functional","Gaussian_pjk")
descriptor[2, c("CA")] <- rep("nSub: 5, nCoeff: 2", 1)

write.table(
  data,
  "data/data_mixtcomp.csv" ,
  sep = ";",
  na = "",
  row.names = F
)
write.table(descriptor,file="data/descriptor.csv",sep=";",na = "")

