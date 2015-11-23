webDemoLearn <- function(folderName)
{
  logConn      <- file(paste(folderName,
                             "out/log.txt",
                             sep = "/"))
  
  IDClassConn <- file(paste(folderName,
                            "out/IDClass.html",
                            sep = "/"))
  
  iniContent <- read.table(paste(folderName,
                                 "in/param.ini",
                                 sep = "/"),
                           sep = "=",
                           strip.white = TRUE,
                           stringsAsFactors = FALSE)
  
  nbClass = iniContent[which(iniContent[, 1] == "nbCluster"), 2]
  
  resGetData <- getData(c(paste(folderName,
                                "in/data.csv",
                                sep = "/"),
                          paste(folderName,
                                "in/descriptor.csv",
                                sep = "/")))
  
  if (nchar(resGetData$warnLog) > 0) # Were there errors when reading the data ?
  {
    cat(resGetData$warnLog,
        file = logConn,
        sep = "")
  }
  else # run can be carried out
  {
    # creation of strategy list
    mcStrategy <- list(nbTrialInInit = 2,
                       nbBurnInIter = 100,
                       nbIter = 100,
                       nbGibbsBurnInIter = 100,
                       nbGibbsIter = 100)
    
    # launch the MixtComp algorithm
    res <- mixtCompCluster(resGetData$lm,
                           mcStrategy,
                           nbClass,
                           0.95)
    
    save(res,
         file = paste(folderName,
                      "out/output.RData",
                      sep = "/"))
    
    writeIDClassJS(res,
                   IDClassConn)
    
    if (nchar(res$mixture$warnLog) > 0)
    {
      cat(res$mixture$warnLog,
          file = logConn,
          sep = "")
      quit(save = "no", status = 1)
    }
    else
    {
      cat("Run completed successfully",
          file = logConn,
          sep = "")
      quit(save = "no", status = 0)
    }
  }
  close(logConn)
  close(IDClassConn)
}