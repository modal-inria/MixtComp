webDemoPredict <- function(folderName)
{
  logConn     <- file(paste(folderName,
                            "out/log.txt",
                            sep = "/"))
  
  IDClassConn <- file(paste(folderName,
                            "out/IDClass.html",
                            sep = "/"))
  
  load(paste(folderName,
             "in/output.RData",
             sep = "/"))
  
  nbClass <- res$mixture$nbCluster
  
  resGetData <-getData(c(paste(folderName,
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
    
    # launch of the MixtComp algorithm
    res <- mixtCompPredict(resGetData$lm,
                           res$variable$param,
                           mcStrategy,
                           nbClass,
                           0.95)
    
    save(res,
         file = paste(folderName,
                      "out/output.RData",
                      sep = "/"))
    
    writeIDClassHTML(res,
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