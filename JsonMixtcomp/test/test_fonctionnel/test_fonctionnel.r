
setwd("/home/etienne/mixtcomp/JsonMixtcomp/test/test_fonctionnel")
source("toolbox_test_fonctionnel.r")
# tests des warnlog

results_warnlog = list()
data_training <- matrix(c(rep(c(1,2),500),rep(c(0,10),250)),nr=500,nc=3)
data_test     <- data_training[1:20,]
data_test[,ncol(data_test)] <- "?"
descriptor    <- matrix(c(rep("Categorical_pjk",2),rep("Poisson_k",1)),nr=1)

colnames(data_training) <- c("C1","C2","X3")
colnames(descriptor)    <- colnames(data_training)
colnames(data_test)     <- colnames(data_training)

res <- test_mixtcomp(data_training,data_test,descriptor)
res$variable$data$X3$completed

test_mixtcomp_by_row(data_training,data_test,descriptor)
 
test_mixtcomp_donnees_func()

