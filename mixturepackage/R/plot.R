#' Plot function.
#' 
#' This function plot the original and Co-clustered data-sets.
#' 
#' @param x output object from \code{\link{cocluster}}.
#' @param y Ignored
#' @param ... Additional argument(s) . Currently we support two additional argument.
#' "asp": If this is set to TRUE the original aspect ratio is conserved. By default "asp" is FALSE.
#' "type" : This is the type of plot which is either "cocluster" or "distribution". The corresponding plots are 
#' Co-clustered data and distributions and mixture densities for Co-clusters respectively. Default is "cocluster" plot.
#'
#' 
#' @importFrom graphics plot
#' @name plot
#' @rdname plot-methods
#' @docType methods
#' @exportMethod plot
#' 
#' 
NULL


#' @rdname plot-methods
#' @aliases plot,BinaryOptions-method
#' 

setMethod(
		f="plot",
		signature = c("BinaryOptions"),
		definition = function(x, y , ...)
		{
      if(x@successful)
      {
        data<-x@data
        clusterdata<-x@coclusterdata
        #min of data
        min <- min(data)
        #max of data
        max <- max(data)
        dimention<-dim(data)
        
        # x11(width=12, height=6)
        layout(matrix(data=c(1,2,3), nrow=1, ncol=3),widths =c(3,6,6))
        
        #par(mfrow=c(1,2))
        Color<- gray((0:1))
        
        ColorLevels <- seq(min, max, length=length(Color))
        # Reverse Y axis
        reverse <- nrow(data):1
        data <- data[reverse,]
        clusterdata <- clusterdata[reverse,]
        
        vararg = list(...)
        
        cocluterplot = TRUE 
        
        if(hasArg("type"))
        {
          if(vararg$type!="cocluster"&&vararg$type!="distribution")
            stop("Invalid plot type, Valid types are:'cocluster' and 'distribution'")
          
          if(vararg$type=="distribution")
            cocluterplot = FALSE
        }
        
        asp = FALSE
        
        if(hasArg(asp))
        {
          if(vararg$asp==TRUE)
            asp = TRUE
        }
        
        if(cocluterplot)
        {
          
          plot(c(0),type="n",xlab="", ylab="", axes=FALSE)
          legend("center",legend =c("0","1"),
                 ,border=F,bg = 'gray90', fill=c("black","white"),title="Legend") #pt.bg=c(1,2))
          # Data 
          #par(mar = c(1,1,1.5,1))
          par(mar = c(1,1,1,1))
          if(asp == FALSE)
            image(t(data), col=Color, axes=FALSE, zlim=c(min,max))
          else
            image(t(data), col=Color, axes=FALSE, zlim=c(min,max), asp=dim(data)[1]/dim(data)[2])
          
          
          title("Original Data")
          # Data cluter 
          par(mar = c(1,1,1,1))
          if(asp == FALSE)
            image(t(clusterdata), col=Color, axes=FALSE, zlim=c(min,max))
          else
            image(t(clusterdata), col=Color, axes=FALSE, zlim=c(min,max), asp=dim(data)[1]/dim(data)[2])
          
          rowvec=1:(x@nbcocluster[1])
          for (i in 1:x@nbcocluster[1]) {
            rowvec[i]=sum(x@rowclass==i-1)/nrow(x@data)
          }
          rowvec[1]
          
          colvec=1:(x@nbcocluster[2])
          for (i in 1:x@nbcocluster[2]) {
            colvec[i]=sum(x@colclass==i-1)/ncol(x@data)
          }
          reverse<-x@nbcocluster[1]:1
          abline(h=cumsum(rowvec[reverse])[1:x@nbcocluster[1]-1],v=cumsum(colvec)[1:x@nbcocluster[2]-1], col="blue",lwd=2)
          
          title("Co-Clustered Data")	
          
        }
        else
        {
          
          ############### Frequencies of classes of binary data #######################
          
          par(mfrow=c(x@nbcocluster[1]+1,x@nbcocluster[2]+1))
          
          clusterdata<-x@coclusterdata 
          # Mixture of columns
          cumSample=0
          for (i in 1:x@nbcocluster[1]){
            cumVar=0
            test=0
            for(j in 1:x@nbcocluster[2]){
              tableBloc=table(clusterdata[(cumSample+1):(cumSample+sum(x@rowclass==i-1)),(cumVar+1):(cumVar+sum(x@colclass==j-1))])
              
              test=rbind(test,tableBloc) #(clusterdata[(cumSample+1):(cumSample+sum(x@rowclass==i-1)),(cumVar+1):(cumVar+sum(x@colclass==j-1))]))
              barplot(tableBloc/sum(tableBloc),xlab=paste("Data values block (",i,",",j,")"),ylab="Frequency",main=paste("Block (",i,",",j,")"),col=c("black","white"))
              #factor(clusterdata[(cumSample+1):(cumSample+sum(x@rowclass==i-1)),(cumVar+1):(cumVar+sum(x@colclass==j-1))]
              cumVar<-cumVar+sum(x@colclass==j-1)
            }
            barplot(test/sum(test),xlab=paste("Data values of row",i),ylab="frequency", main=paste("Mixture of row",i))
            cumSample<-cumSample+sum(x@rowclass==i-1)
          }
          
          # Mixture of rows
          cumVar=0
          for (j in 1:x@nbcocluster[2]){
            cumSample=0
            temp=0
            for(i in 1:x@nbcocluster[1]){
              temp=rbind(temp,table(clusterdata[(cumSample+1):(cumSample+sum(x@rowclass==i-1)),(cumVar+1):(cumVar+sum(x@colclass==j-1))]))
              cumSample<-cumSample+sum(x@rowclass==i-1)
            }
            barplot(temp/sum(temp),xlab=paste("Data values of column",j),ylab="Frequency", main=paste("Mixture of column",j))
            cumVar<-cumVar+sum(x@colclass==j-1)
          }
          barplot(table(clusterdata)/sum(table(clusterdata)),xlab="Data values",ylab="Frequency",
                  main="Final mixture", col=c("black","white"))
          
          title(main="Histogram/density for each block",outer=TRUE,line=-1)
        }
        
      }
      else{
        cat("Co-Clustering was not successful.\n")
      }
		}
)


#' @rdname plot-methods
#' @aliases plot,ContingencyOptions-method
#'

setMethod(
		f="plot",
		signature = c("ContingencyOptions"),
		definition = function(x,y,...) {
      if(x@successful)
      {
        data<-x@data
        clusterdata<-x@coclusterdata
        min <- min(data)
        max <- max(data)
        dimention<-dim(data)
        
        
        reverse <- nrow(data):1
        data <- data[reverse,]
        clusterdata <- clusterdata[reverse,]
        
        
        
        vararg = list(...)
        
        cocluterplot = TRUE 
        if(hasArg("type"))
        {
          if(vararg$type!="cocluster"&&vararg$type!="distribution")
            stop("Invalid plot type, Valid types are:'cocluster' and 'distribution'")
          
          if(vararg$type=="distribution")
            cocluterplot = FALSE
        }
        
        asp = FALSE
        
        if(hasArg(asp))
        {
          if(vararg$asp==TRUE)
            asp = TRUE
        }
        
        if(cocluterplot)
        {
          layout(matrix(data=c(1,2,3), nrow=1, ncol=3),widths =c(6,6,1))
          
          Color<- gray((0:256)/256)
          
          ColorLevels <- seq(min, max, length=length(Color))
          
          # Data 
          par(mar = c(1,1,1,1))
          if(asp == FALSE)
            image(t(data), col=Color, axes=FALSE, zlim=c(min,max))
          else
            image(t(data), col=Color, axes=FALSE, zlim=c(min,max), asp=dim(data)[1]/dim(data)[2])
          title("Original Data")
          
          
          # Data cluter 
          par(mar = c(1,1,1,1))
          if(asp == FALSE)
            image(t(clusterdata), col=Color, axes=FALSE, zlim=c(min,max))
          else
            image(t(clusterdata), col=Color, axes=FALSE, zlim=c(min,max), asp=dim(data)[1]/dim(data)[2])
          
          
          rowvec=1:(x@nbcocluster[1])
          for (i in 1:x@nbcocluster[1]) {
            rowvec[i]=sum(x@rowclass==i-1)/nrow(x@data)
          }
          
          colvec=1:(x@nbcocluster[2])
          for (i in 1:x@nbcocluster[2]) {
            colvec[i]=sum(x@colclass==i-1)/ncol(x@data)
          }
          reverse<-x@nbcocluster[1]:1
          abline(h=cumsum(rowvec[reverse])[1:x@nbcocluster[1]-1],v=cumsum(colvec)[1:x@nbcocluster[2]-1],col="blue",lwd=2)
          title("Co-Clustered Data")
          
          par(mar = c(1,2,1.5,1))
          image(1, ColorLevels,
                matrix(data=ColorLevels, ncol=length(ColorLevels),nrow=1),
                col=Color,xaxt="n")
          title("Scale")
          
          
        }
        else
        {
          ###########  barplot of contingency data  ##########################
          
          par(mfrow=c(x@nbcocluster[1]+1,x@nbcocluster[2]+1))
          
          clusterdata<-x@coclusterdata 
          
          cumSample=0
          for (i in 1:x@nbcocluster[1]){
            
            cumVar=0
            for(j in 1:x@nbcocluster[2]){
              blocTable=clusterdata[(cumSample+1):(cumSample+sum(x@rowclass==i-1)),(cumVar+1):(cumVar+sum(x@colclass==j-1))]
              barplot(table(blocTable)/sum(table(blocTable)), main=paste("Block (",i,",",j,")"), 
                      xlab=paste("Data values of block (",i,",",j,")"), col="lightblue",ylab="Frequency")
              cumVar<-cumVar+sum(x@colclass==j-1) 
            }
            rowBloc<-clusterdata[(cumSample+1):(cumSample+sum(x@rowclass==i-1)), 1:(dim(clusterdata)[2])]
            barplot(table(rowBloc)/sum(table(rowBloc)),col="lightblue",xlab=paste("Data values of row",i),ylab="Frequency")
            
            cumSample<-cumSample+sum(x@rowclass==i-1)
          }
          
          cumVar=0
          
          for (k in 1:x@nbcocluster[2]){
            
            colBloc=table(clusterdata[1:dim(clusterdata)[1],(cumVar+1):(cumVar+sum(x@colclass==k-1))])
            barplot(colBloc/sum(colBloc),ylab="Frequency",xlab=paste("Data values of column",k),
                    col="lightblue", main=paste("Mixture density of column",k))
            
            cumVar<-cumVar+sum(x@colclass==k-1)
          }
          barplot(table(clusterdata)/sum(table(clusterdata)), xlab=paste("Data values"),ylab="Frequency", 
                  col="lightblue",main="Final mixture density")
          
          title(main="Histograms of classes of contingency data",outer=TRUE,line=-1)
          
        }
      }
      else{
        cat("Co-Clustering was not successful.\n")
      }
      
      }
)

#' @rdname plot-methods
#' @aliases plot,ContinuousOptions-method
#'
setMethod(
		f="plot",
		signature = c("ContinuousOptions"),
		definition = function(x,y,...) {
		  if(x@successful)
		  {
		    vararg = list(...)
		    cocluterplot = TRUE 
		    if(hasArg("type"))
		    {
		      if(vararg$type!="cocluster"&&vararg$type!="distribution")
		        stop("Invalid plot type, Valid types are:'cocluster' and 'distribution'")
		      
		      if(vararg$type=="distribution")
		        cocluterplot = FALSE
		    }
		    
		    asp = FALSE
		    
		    if(hasArg(asp))
		    {
		      if(vararg$asp==TRUE)
		        asp = TRUE
		    }
		    
		    data<-x@data
		    clusterdata<-x@coclusterdata
		    
		    min <- min(data)
		    max <- max(data)
		    dimention<-dim(data)
		    
		    
		    reverse <- nrow(data):1
		    data <- data[reverse,]
		    clusterdata <- clusterdata[reverse,]
		    
		    
		    if (cocluterplot) 
		    {
		      layout(matrix(data=c(1,2,3), nrow=1, ncol=3),widths =c(6,6,1))
		      Color<- gray((0:256)/256)
		      
		      ColorLevels <- seq(min, max, length=length(Color))
		      
		      # Data 
		      par(mar = c(1,1,1,1))
		      if(asp == FALSE)
		        image(t(data), col=Color, axes=FALSE, zlim=c(min,max))
		      else
		        image(t(data), col=Color, axes=FALSE, zlim=c(min,max), asp=dim(data)[1]/dim(data)[2])
		      title("Original Data")
		      
		      
		      # Data cluter 
		      par(mar = c(1,1,1,1))
		      if(asp == FALSE)
		        image(t(clusterdata), col=Color, axes=FALSE, zlim=c(min,max))
		      else
		        image(t(clusterdata), col=Color, axes=FALSE, zlim=c(min,max), asp=dim(data)[1]/dim(data)[2])
		      
		      rowvec=1:(x@nbcocluster[1])
		      for (i in 1:x@nbcocluster[1]) {
		        rowvec[i]=sum(x@rowclass==i-1)/nrow(x@data)
		      }
		      
		      colvec=1:(x@nbcocluster[2])
		      for (i in 1:x@nbcocluster[2]) {
		        colvec[i]=sum(x@colclass==i-1)/ncol(x@data)
		      }
		      reverse<-x@nbcocluster[1]:1
		      abline(h=cumsum(rowvec[reverse])[1:x@nbcocluster[1]-1],v=cumsum(colvec)[1:x@nbcocluster[2]-1],col="blue",lwd=2)
		      
		      title("Co-Clustered Data")
		      
		      par(mar = c(1,2,1.5,1))
		      image(1, ColorLevels,
		            matrix(data=ColorLevels, ncol=length(ColorLevels),nrow=1),
		            col=Color,xaxt="n")
		      
		      title("Scale")
		      
		    } 
		    else 
		    {
		      ################ The graph of each block and the mixture ##########################
		      
		      # get dimension			
		      nbRowCluster=x@nbcocluster[1];
		      nbColCluster=x@nbcocluster[2];
		      # compute the number of varaibles in each  block
		      nbVar=vector(length=nbColCluster);
		      for(j in 1:nbColCluster)
		      {
		        nbVar[j] = sum(x@colclass==j-1);
		      }
		      # compute the number of sample in each  block
		      nbSample=vector(length=nbRowCluster);
		      for (i in 1:nbRowCluster)
		      {
		        nbSample[i] = sum(x@rowclass==i-1);
		      }
		      # seprate the graphic device
		      par(mfrow=c(nbRowCluster+1,nbColCluster+1))
		      cumSample=0
		      clusterdata<-x@coclusterdata
		      # main loop
		      colorBloc=1
		      rowcolorMixt=1
		      
		      for (i in 1:nbRowCluster)
		      {
		        densMixt=0
		        cumVar=0
		        mindata = min(clusterdata[(cumSample+1):(cumSample+nbSample[i]),1:(dim(clusterdata)[2])])
		        maxdata = max(clusterdata[(cumSample+1):(cumSample+nbSample[i]),1:(dim(clusterdata)[2])])
		        xaxis =seq( mindata, maxdata , by=0.0001)
		        densMixt=vector(length = length(xaxis))
		        
		        for(j in 1:nbColCluster)
		        {
		          tableBloc<-clusterdata[(cumSample+1):(cumSample+nbSample[i]), (cumVar+1):(cumVar+nbVar[j])]
		          hist(tableBloc,xlab=paste("Data values of block (",i,",",j,")"),
		               col="lightblue",ylab="Frequency", main=paste("Block (",i,",",j,")"),proba=TRUE)	
		          
		          densBloc <- dnorm( xaxis,x@classmean[i,j],sqrt(x@classvariance[i,j])) 
		          densMixt = densMixt + x@columnproportions[j]*densBloc
		          points(x=xaxis, y=densBloc,type='l',lty=1, col=colorBloc+1) 
		          cumVar<-cumVar+nbVar[j]
		          colorBloc=colorBloc+1
		        }
		        
		        # Graph of the mixture of columns
		        colBloc<-clusterdata[(cumSample+1):(cumSample+nbSample[i]), 1:(dim(clusterdata)[2])]
		        h<-hist(colBloc,col="lightblue",xlab=paste("Data values of row",i),ylab="Frequency", main=paste("Mixture density of row",i),proba=TRUE)
		        points(xaxis,densMixt,col="black",type='l',lty=1)
		        
		        for(l in 1:nbColCluster)
		        { 
		          densBloc<-x@columnproportions[l]*dnorm( xaxis,x@classmean[i,l],sqrt(x@classvariance[i,l])) 
		          points( xaxis,densBloc,type='l',lty=1,col=rowcolorMixt+1) 
		          rowcolorMixt=rowcolorMixt+1
		        }    
		        cumSample<-cumSample+nbSample[i]
		      }
		      
		      ### bloc columns
		      # Graph of the mixture of rows
		      cumVar=0 
		      colColor=2
		      
		      for(j in 1:nbColCluster)
		      {
		        densMixt=0
		        rowTable<-clusterdata[1:(dim(clusterdata)[1]),(cumVar+1):(cumVar+nbVar[j])]
		        h<-hist(rowTable, xlab=paste("Data values of column",j),ylab="Frequency",
		                col="lightblue", main=paste("Mixture density of column",j),proba=TRUE) 
		        
		        xaxis=seq(min(rowTable), max(rowTable), by=0.0001)
		        cl=colColor
		        for(i in 1:nbRowCluster)
		        {
		          densBloc<-x@rowproportions[i]*dnorm( xaxis,x@classmean[i,j],sqrt(x@classvariance[i,j]))
		          densMixt=densMixt+ densBloc 
		          points( xaxis,densBloc,type='l',lty=1,col=cl) 
		          cl=nbColCluster+colColor
		        }
		        colColor=colColor+1
		        par(new=TRUE, ann= FALSE)
		        points( xaxis,densMixt,col="black",type='l',lty=1)
		        cumVar<-cumVar+nbVar[j]
		      }
		      
		      # All mixtures of rows and columns
		      hist(clusterdata,xlab=paste("Data values"),ylab="Frequency", 
		           col="lightblue",main="Final mixture density",proba=TRUE)
		      
		      xaxis=seq(min(clusterdata),max(clusterdata), by=0.0001)
		      densMixt=0
		      colorMixt=1
		      for(i in 1:nbRowCluster){
		        for(j in 1:nbColCluster){
		          densBloc<-x@rowproportions[i]*x@columnproportions[j]*dnorm(xaxis,x@classmean[i,j],sqrt(x@classvariance[i,j]))
		          densMixt<-densMixt+densBloc
		          points(xaxis,densBloc, col=colorMixt+1,type='l',lty=1)
		          colorMixt=colorMixt+1
		        }
		      }
		      par(new=TRUE, ann= FALSE)
		      points(xaxis,densMixt,col="black",type='l',lty=1)#,yaxt ="n",xaxt="n", xlab="",ylab="", main="")
		      
		      title(main="Histograms and densities of each block",outer=TRUE,line=-1)  
		      
		    }
		    
		  }
		  else{
		    cat("Co-Clustering was not successful.\n")
		  }

		}
)

#' @rdname plot-methods
#' @aliases plot,CategoricalOptions-method
#'

setMethod(
  f="plot",
  signature = c("CategoricalOptions"),
  definition = function(x,y,...) {
    if(x@successful)
    {
      data<-x@data
      clusterdata<-x@coclusterdata
      min <- min(data)
      max <- max(data)
      dimention<-dim(data)
      
      
      reverse <- nrow(data):1
      data <- data[reverse,]
      clusterdata <- clusterdata[reverse,]
      
      
      
      vararg = list(...)
      
      cocluterplot = TRUE 
      if(hasArg("type"))
      {
        if(vararg$type!="cocluster"&&vararg$type!="distribution")
          stop("Invalid plot type, Valid types are:'cocluster' and 'distribution'")
        
        if(vararg$type=="distribution")
          cocluterplot = FALSE
      }
      
      asp = FALSE
      
      if(hasArg(asp))
      {
        if(vararg$asp==TRUE)
          asp = TRUE
      }
      
      if(cocluterplot)
      {
        layout(matrix(data=c(1,2,3), nrow=1, ncol=3),widths =c(6,6,1))
        
        Color<- gray((0:256)/256)
        
        ColorLevels <- seq(min, max, length=length(Color))
        
        # Data 
        par(mar = c(1,1,1,1))
        if(asp == FALSE)
          image(t(data), col=Color, axes=FALSE, zlim=c(min,max))
        else
          image(t(data), col=Color, axes=FALSE, zlim=c(min,max), asp=dim(data)[1]/dim(data)[2])
        title("Original Data")
        
        
        # Data cluter 
        par(mar = c(1,1,1,1))
        if(asp == FALSE)
          image(t(clusterdata), col=Color, axes=FALSE, zlim=c(min,max))
        else
          image(t(clusterdata), col=Color, axes=FALSE, zlim=c(min,max), asp=dim(data)[1]/dim(data)[2])
        
        
        rowvec=1:(x@nbcocluster[1])
        for (i in 1:x@nbcocluster[1]) {
          rowvec[i]=sum(x@rowclass==i-1)/nrow(x@data)
        }
        
        colvec=1:(x@nbcocluster[2])
        for (i in 1:x@nbcocluster[2]) {
          colvec[i]=sum(x@colclass==i-1)/ncol(x@data)
        }
        reverse<-x@nbcocluster[1]:1
        abline(h=cumsum(rowvec[reverse])[1:x@nbcocluster[1]-1],v=cumsum(colvec)[1:x@nbcocluster[2]-1],col="blue",lwd=2)
        title("Co-Clustered Data")
        
        par(mar = c(1,2,1.5,1))
        image(1, ColorLevels,
              matrix(data=ColorLevels, ncol=length(ColorLevels),nrow=1),
              col=Color,xaxt="n")
        title("Scale")
      }
      else
      {
        
        ############### Frequencies of classes of categorical data #######################
        
        par(mfrow=c(x@nbcocluster[1]+1,x@nbcocluster[2]+1))
        
        clusterdata<-x@coclusterdata 
        # Mixture of columns
        cumSample=0
        for (i in 1:x@nbcocluster[1]){
          cumVar=0
          test=0
          for(j in 1:x@nbcocluster[2]){
            tableBloc=table(clusterdata[(cumSample+1):(cumSample+sum(x@rowclass==i-1)),(cumVar+1):(cumVar+sum(x@colclass==j-1))])
            
            test=rbind(test,tableBloc) #(clusterdata[(cumSample+1):(cumSample+sum(x@rowclass==i-1)),(cumVar+1):(cumVar+sum(x@colclass==j-1))]))
            barplot(tableBloc/sum(tableBloc),xlab=paste("Data values block (",i,",",j,")"),ylab="Frequency",main=paste("Block (",i,",",j,")"))
            #factor(clusterdata[(cumSample+1):(cumSample+sum(x@rowclass==i-1)),(cumVar+1):(cumVar+sum(x@colclass==j-1))]
            cumVar<-cumVar+sum(x@colclass==j-1)
          }
          barplot(test/sum(test),xlab=paste("Data values of row",i),ylab="frequency", main=paste("Mixture of row",i))
          cumSample<-cumSample+sum(x@rowclass==i-1)
        }
        
        # Mixture of rows
        cumVar=0
        for (j in 1:x@nbcocluster[2]){
          cumSample=0
          temp=0
          for(i in 1:x@nbcocluster[1]){
            temp=rbind(temp,table(clusterdata[(cumSample+1):(cumSample+sum(x@rowclass==i-1)),(cumVar+1):(cumVar+sum(x@colclass==j-1))]))
            cumSample<-cumSample+sum(x@rowclass==i-1)
          }
          barplot(temp/sum(temp),xlab=paste("Data values of column",j),ylab="Frequency", main=paste("Mixture of column",j))
          cumVar<-cumVar+sum(x@colclass==j-1)
        }
        barplot(table(clusterdata)/sum(table(clusterdata)),xlab="Data values",ylab="Frequency",
                main="Final mixture")
        
        title(main="Histogram/density for each block",outer=TRUE,line=-1)
      }
      
    }
    else{
      cat("Co-Clustering was not successful.\n")
    }
    
  }
)