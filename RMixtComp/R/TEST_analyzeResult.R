confidenceInterval <- function(dataOut, nbSamples, level = 0.95)
{
  listVars <- ls(dataOut[[2]]$data)
  intData <- list()
  z <- qnorm((1. + level) / 2.)
  for (currVar in listVars)
  {
    cat(currVar, "\n")
    currData <- as.character(dataOut[[2]]$data[[currVar]]$completed)
    if (grepl("categorical", currVar))
    {
      for (i in 1:nrow(dataOut[[2]]$data[[currVar]]$posMiss))
      {
        listChar <- vector(mode = "character")
        cumProba <- 0.
        classOrder <- order(dataOut[[2]]$data[[currVar]]$statMissing[i,],
                            decreasing = TRUE)
        for (k in 1:length(classOrder))
        {
          if (cumProba < level)
          {
            currProba <- dataOut[[2]]$data[[currVar]]$statMissing[i, classOrder[k]]
            cumProba <- cumProba + currProba
            listChar[[length(listChar)+1]] <- paste(classOrder[k], currProba, sep = ":")
          }
        }
        currData[dataOut[[2]]$data[[currVar]]$posMiss[i]] <- paste(listChar, collapse = " ")
      }
      intData[[currVar]] <- currData
    }
    if (grepl("gaussian", currVar))
    {
      for (i in 1:nrow(dataOut[[2]]$data[[currVar]]$posMiss))
      {
        listChar <- vector(mode = "character")
        mean <- dataOut[[2]]$data[[currVar]]$statMissing[i, 1]
        sd <- dataOut[[2]]$data[[currVar]]$statMissing[i, 2]
        currData[dataOut[[2]]$data[[currVar]]$posMiss[i]] <- paste("[",
                                                                   mean - z * sd / sqrt(nbSamples),
                                                                   ":",
                                                                   mean + z * sd / sqrt(nbSamples),
                                                                   "]",
                                                                   sep = "")
      }
      intData[[currVar]] <- currData
    }
  }
  write.table(intData,
              quote = FALSE,
              sep = ";",
              file = "out/confidenceInterval.csv",
              row.names=FALSE,
              col.names=TRUE)
}

exportParam <- function(dataOut)
{
  write.table(dataOut[[1]]@results@partition,
              file = "out/classOut.csv",
              row.names=FALSE,
              col.names=FALSE)
  
  listVars <- ls(dataOut[[2]]$data)
  listData <- list()
  for (currVar in listVars)
  {
    fileName <- paste("out/",
                      currVar,
                      "param.csv",
                      sep = "")
    currData <- list(currVar = dataOut[[2]]$param[[currVar]]$param)
    write.table(currData,
                file = fileName,
                sep = ";",
                row.names = FALSE,
                col.names = TRUE)
  }
}

completedData <- function(dataOut)
{
  write.table(dataOut[[1]]@results@partition,
              file = "out/classOut.csv",
              row.names=FALSE,
              col.names=FALSE)
  listVars <- ls(dataOut[[2]]$data)
  listData <- list()
  for (currVar in listVars)
  {
    listData[[currVar]] <- dataOut[[2]]$data[[currVar]]$completed

  }
  write.table(listData,
              file = "out/completedData.csv",
              sep = ";",
              row.names = FALSE,
              col.names = TRUE)
}

export2DPoints <- function(nbIterations)
{
  classIn <- read.table("dataGen/learn/classIn.csv",
                        sep = ";")
  nbInd <- length(classIn$V1)
  
  for (i in -1:(nbIterations-1))
  {
    nbMisClass <- 0
    
    fileNamez_i <- paste("out/log/composer-",
                         i,
                         "-z_i.csv",
                         sep = "")
    z_i <- read.table(fileNamez_i,
                      sep = ";")
    
    fileNameVar1 <- paste("out/log/gaussian1-",
                          i,
                          "-data.csv",
                          sep = "")
    var1 <- read.table(fileNameVar1,
                       sep = ";")
    
    fileNameVar2 <- paste("out/log/gaussian2-",
                          i,
                          "-data.csv",
                          sep = "")
    var2 <- read.table(fileNameVar2,
                       sep = ";")
    
    param1 <- read.table(paste("out/log/gaussian1-",
                               i,
                               "-param.csv",
                               sep = ""),
                         sep = ";")
    param2 <- read.table(paste("out/log/gaussian2-",
                               i,
                               "-param.csv",
                               sep = ""),
                         sep = ";")
    
    for (ind in 1:nbInd)
    {
      if ((z_i$V1[ind] + 1) != classIn$V1[ind])
      {
        nbMisClass = nbMisClass + 1
      }
    }
    
    colPool <- c("red", "blue", "green")
    png(paste("out/graph/", sprintf("%04d", i), ".png", sep = ""),
        width = 1000,
        height = 1000)
    plot(var1$V1,
         var2$V1,
         ylim = c(-0.75, 0.75),
         xlim = c(-0.75, 0.75),
         col = colPool[z_i$V1 + 1])
    plotellipse(mid = c(param1$V1[1],
                        param2$V1[1]),
                rx = param1$V1[2],
                ry = param2$V1[2])
    plotellipse(mid = c(param1$V1[3],
                        param2$V1[3]),
                rx = param1$V1[4],
                ry = param2$V1[4])    
#    text((param1$V1[1]+param1$V1[3])/2.,
#         (param2$V1[1]+param2$V1[3])/2.,
    text(-0.50,
          0.50,
         paste("Misclassified / Total: ", nbMisClass, " / ", nbInd, "\n",
               "Misclassification rate: ", nbMisClass / nbInd, "\n",
               "Var 1:\n",
               "k=1 - exp: ", param1$V1[1], "\n",
               "k=1 - sd: ", param1$V1[2], "\n",
               "k=2 - exp: ", param1$V1[3], "\n",
               "k=2 - sd: ", param1$V1[4], "\n",
               "Var 2:\n",
               "k=1 - exp: ", param2$V1[1], "\n",
               "k=1 - sd: ", param2$V1[2], "\n",
               "k=2 - exp: ", param2$V1[3], "\n",
               "k=2 - sd: ", param2$V1[4], "\n",
               sep = ""),
         adj = 0.)
    
    dev.off()
  }
}

exportMisClass <- function(nbIterations)
{
  classIn <- read.table("dataGen/classIn.csv",
                        sep = ";")
  nbInd <- length(classIn$V1)
  
  misClass <- vector(mode = "integer", length = nbIterations)
  
  for (i in -1:(nbIterations-1))
  {
    fileNamez_i <- paste("out/log/composer-",
                         i,
                         "-z_i.csv",
                         sep = "")
    z_i <- read.table(fileNamez_i,
                      sep = ";")
    
    nbMisClass <- 0
    for (ind in 1:nbInd)
    {
      if ((z_i$V1[ind] + 1) != classIn$V1[ind])
      {
        nbMisClass = nbMisClass + 1
      }
    }
    misClass[i + 1] <- nbMisClass
  }
  
  png("out/misClass.png",
      width = 1000,
      height = 1000)
  plot(misClass,
#       ylim = c(0., 50.),
       type = "l")
  dev.off()
}

plotFunctional <- function(i) {
  df <- read.csv(file = "dataGenNew/learn/data.csv",
                 header = TRUE,
                 sep = ";",
                 as.is = TRUE)
  lsSplit <- strsplit(df$Functional1[[i]], ",")
  
  mySplit <- function(x) {
    return(strsplit(x, ":")[[1]])
  }
  
  dataSplit <- lapply(lsSplit[[1]], mySplit)
  
  nPoints <- length(dataSplit)
  x <- vector("numeric", nPoints)
  y <- vector("numeric", nPoints)
  
  for (i in 1:nPoints) {
    x[i] <- as.numeric(dataSplit[[i]][1])
    y[i] <- as.numeric(dataSplit[[i]][2])
  }
  
  plot(x, y)
}
