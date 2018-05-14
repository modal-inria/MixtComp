n_row <- 10000
n_miss_per_col <- n_row*0.2
# data_ordinal <- matrix(nc=3,c(sample(1:n_row,replace=F,size = n_row),sample(1:n_row,replace=F,size = n_row),sample(1:n_row,replace=F,size = n_row)),byrow=F)
data_gaussian <- matrix(nc=3,rnorm(n = 3*n_row,mean = c(5,10,100)),byrow=T)
data_training <- do.call("rbind", replicate(1000, read.csv("/home/etienne/cylande/retfor/livrable_2/data/working_data/data_training.csv",sep=";"), simplify = FALSE))
data_categorical <- apply(data_training[1:n_row,c(1:3)],2,function(x){as.numeric(as.factor(x))})
data_poisson <- data_training[1:n_row,c(36:38)]

data_json <- cbind(data_categorical,data_poisson,data_gaussian)
colnames(data_json) <- c("categorical1","categorical2","categorical3","poisson1","poisson2","poisson3","gaussian1","gaussian2","gaussian3")
descriptor <- matrix(nr=1,c(rep("Categorical_pjk",3),rep("Poisson_k",3),rep("Gaussian_sjk",3)))
colnames(descriptor) <- colnames(data_json)

for(i in 1:ncol(data_json)){
  data_json[sample(1:n_row,size=n_miss_per_col,replace = F),i] <- "?"
}

# data_json <- read.table(file="/home/etienne/cylande/mixtcomp/JsonMixtcomp/data/utest/data_test_size.csv",sep=";",header=T)
data_json <- apply(data_json,2,as.character)
write.table(data_json,file="/home/etienne/cylande/mixtcomp/JsonMixtcomp/utest/data_test_size.csv",sep=";",row.names = F,quote = T)
write.table(descriptor,file="/home/etienne/cylande/mixtcomp/JsonMixtcomp/utest/data/desc_test_size.csv",sep=";",row.names = F,quote = T)

require(RMixtComp)
dataPath <- "/home/etienne/cylande/mixtcomp/JsonMixtcomp/data/utest/data_test_size.csv"
descriptorPath <-"/home/etienne/cylande/mixtcomp/JsonMixtcomp/data/utest/desc_test_size.csv"

resGetData <- getData(c(dataPath,descriptorPath))

for(i in 1:length(resGetData$lm)){
  resGetData$lm[[i]]$data <- as.character(resGetData$lm[[i]]$data)
}
mcStrategy = list(nbBurnInIter=10,nbIter=10,nbGibbsBurnInIter=10,nbGibbsIter=10,parameterEdgeAuthorized=T)
require(jsonlite)
arg_list_json <- toJSON(list(resGetData_lm = resGetData$lm,mcStrategy=mcStrategy,nbClass = 5,confidenceLevel = 0.95,mode="learn"),auto_unbox = T)
write(x = arg_list_json,"/home/etienne/cylande/mixtcomp/JsonMixtcomp/utest/data/arg_list_test_size.json")

################## run Rmixtcomp

source("cylande/retfor/livrable_2/tools/tools_mixtcomp.r")
t1 <- system.time({
  param_mixtcomp <- mixtcomp_learning(nbClass=5, dataPath, descriptorPath,n_iter=20,n_burnin_iter=20,gibbs_n_iter=20,gibbs_n_burnin_iter=20,n_parallel_mixtcomp=1)
})

print(t1)

t2 <- system.time({
  system("/home/etienne/cylande/mixtcomp/JsonMixtcomp/JsonMixtcomp /home/etienne/cylande/mixtcomp/JsonMixtcomp/utest/data/arg_list_test_size.json /home/etienne/cylande/mixtcomp/JsonMixtcomp/utest/data/output_test_size.json")
})
print(t2)
cat("temps Rmixtcomp: ")
cat(t1["elapsed"])
cat("\n temps JsonMixtcomp: ")
cat(t2["elapsed"])

file.remove("/home/etienne/cylande/mixtcomp/JsonMixtcomp/utest/data/arg_list_test_size.json")
file.remove("/home/etienne/cylande/mixtcomp/JsonMixtcomp/utest/data/output_test_size.json")
file.remove("/home/etienne/cylande/mixtcomp/JsonMixtcomp/utest/data/data_test_size.csv")
file.remove("/home/etienne/cylande/mixtcomp/JsonMixtcomp/utest/data/desc_test_size.csv")


