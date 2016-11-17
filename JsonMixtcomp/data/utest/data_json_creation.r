data_ordinal <- matrix(nr=10,c(sample(1:10,replace=F,size = 10),sample(1:10,replace=F,size = 10),sample(1:10,replace=F,size = 10)),byrow=F)
data_gaussian <- matrix(nc=3,rnorm(n = 30,mean = c(5,10,100)),byrow=T)
data_training <- read.csv("data/working_data/data_training.csv",sep=";")
data_categorical <- apply(data_training[,c(1:3)],2,function(x){as.numeric(as.factor(x))})
data_poisson <- data_training[,c(36:38)]

data_json <- cbind(data_categorical,data_poisson,data_ordinal,data_gaussian)
colnames(data_json) <- c("categorical1","categorical2","categorical3","poisson1","poisson2","poisson3","ordinal1","ordinal2","ordinal3","gaussian1","gaussian2","gaussian3")
descriptor <- matrix(nr=1,c(rep("Categorical_pjk",3),rep("Poisson_k",3),rep("Ordinal",3),rep("Gaussian_sjk",3)))
colnames(descriptor) <- colnames(data_json)

for(i in 1:nrow(data_json)){
  data_json[i,sample(setdiff(1:ncol(data_json),7:9),size=2,replace = F)] <- "?"
}

write.table(data_json,file="/home/etienne/cylande/mixtcomp/JsonMixtcomp/data/utest/data_utest.csv",sep=";",row.names = F,quote = F)
write.table(descriptor,file="/home/etienne/cylande/mixtcomp/JsonMixtcomp/data/utest/desc_utest.csv",sep=";",row.names = F)


require(RMixtComp)
resGetData <- getData(c("/home/etienne/cylande/mixtcomp/JsonMixtcomp/data/utest/data_utest.csv",
                        "/home/etienne/cylande/mixtcomp/JsonMixtcomp/data/utest/desc_utest.csv"))

arg_list_json <- toJSON(list(resGetData_lm = resGetData$lm,mcStrategy=mcStrategy,nbClass = 5,confidenceLevel = 0.95),auto_unbox = T,)
write(x = arg_list_json,"/home/etienne/cylande/mixtcomp/JsonMixtcomp/data/utest/arg_list_utest.json")
