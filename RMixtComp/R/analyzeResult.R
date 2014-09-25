confidenceInterval <- function(dataOut, level = 0.95)
{
  listVars <- ls(dataOut[[2]]$data)
  intData <- list()
  for (currVar in listVars)
  {
    cat(currVar, "\n")
    currData <- as.character(dataOut[[2]]$data[[currVar]]$completed)
    if (grepl("categorical", currVar))
    {
      cat(length(dataOut[[2]]$data[[currVar]]$posMiss), "\n")
      for (i in 1:nrow(dataOut[[2]]$data[[currVar]]$posMiss))
      {
        listChar <- vector(mode = "character")
        cumProba <- 0.
        cat(i, "\n")
        classOrder <- order(dataOut[[2]]$data[[currVar]]$statMissing[i,],
                            decreasing = TRUE)
        for (k in 1:length(classOrder))
        {
          currProba <- dataOut[[2]]$data[[currVar]]$statMissing[i, classOrder[k]]
          cumProba <- cumProba + currProba
          if (k == 1 | cumProba < level)
          {
            listChar[[length(listChar)+1]] <- paste(classOrder[k], currProba, sep = ":")
          }
        }
        currData[dataOut[[2]]$data[[currVar]]$posMiss[i]] <- paste(listChar, collapse = " ")
      }
      intData[[currVar]] <- currData
    }
    if (grepl("gaussian", currVar))
    {
    }
  }
  write.table(intData,
              quote = FALSE,
              sep = ";",
              file = "out/confidenceInterval.csv",
              row.names=FALSE,
              col.names=TRUE)
}

completedData <- function(dataOut)
{
  write.table(dataOut[[1]]@results@partition,
              file = "out/classOut.csv",
              row.names=FALSE,
              col.names=FALSE)
  
  write.table(data.frame(categorical1 = dataOut[[2]]$data$categorical1$completed),
              file = "out/categorical1data.csv",
              row.names = FALSE,
              col.names = FALSE)
  write.table(data.frame(categorical1 = dataOut[[2]]$param$categorical1),
              file = "out/categorical1param.csv",
              sep = ";",
              row.names = FALSE,
              col.names = TRUE)
  
  write.table(data.frame(gaussian1 = dataOut[[2]]$data$gaussian1$completed),
              file = "out/gaussian1data.csv",
              row.names = FALSE,
              col.names = FALSE)
  write.table(data.frame(categorical1 = dataOut[[2]]$param$gaussian1),
              file = "out/gaussian1param.csv",
              sep = ";",
              row.names = FALSE,
              col.names = TRUE)
}

exportMap <- function(nbIterations)
{
  classIn <- read.table("dataGen/classIn.csv",
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
    png(paste("out/graph/", i, ".png", sep = ""),
        width = 1000,
        height = 1000)
    plot(var1$V1,
         var2$V1,
         #         ylim = c(-150., 150.),
         #         xlim = c(-150., 150.),
         col = colPool[z_i$V1 + 1])
    text(-20,
         0,
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

exportGraph2 <- function(nbIterations)
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
  plot(misClass,
       ylim = c(0., 50.),
       type = "l")
}