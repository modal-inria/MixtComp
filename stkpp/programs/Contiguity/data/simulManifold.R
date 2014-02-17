'pcaManifold' <- function(filename, n, p=2, bound=2)
{
  x<-matrix(runif(2*n,-bound, bound), nrow=n, ncol=2);
  z<- ((x[,1]^p + x[,2]^p));
  s<-cbind(x,z)
  pca<-prcomp(s)
  print(pca$rotation)
  plot3d( x[,1], x[,2], z, type = "p", pch='+', cex = 0.1, aspect=TRUE)
  planes3d(a=pca$rotation[1,3], b=pca$rotation[2,3], c=pca$rotation[3,3], d=-bound/2, alpha = 0.5)
  write.table(x=s,file=filename,quote=FALSE,append=FALSE,row.names=FALSE,col.names=FALSE, sep= ",")
}

'localPcaManifold' <- function(filename, bound)
{
  axis<-read.csv(filename, header=FALSE, sep= ",", dec='.', )
  print(axis)
  planes3d(a=axis[1,3], b=axis[2,3], c=axis[3,3], d=-bound/2, alpha = 0.3)
}