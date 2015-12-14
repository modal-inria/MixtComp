webDemoPredict <- function(folderName)
{
  RDataFileIn <- paste(folderName,
                       "in/output.RData",
                       sep = "/")
  
  dataFile <- paste(folderName,
                    "in/data.csv",
                    sep = "/")
  
  descFile <- paste(folderName,
                    "in/descriptor.csv",
                    sep = "/")
  
  logFile <- paste(folderName,
                   "out/log.txt",
                   sep = "/")
  
  IDHTMLFile <- paste(folderName,
                      "out/IDClass.html",
                      sep = "/")
  
  RDataFileOut <- paste(folderName,
                        "out/output.RData",
                        sep = "/")
  
  checkDimLog <- validDim(dataFile)
  if (nchar(checkDimLog) > 0)
  {
    cat(checkDimLog,
        file = logFile,
        sep = "",
        append = TRUE)
    quit(save = "no", status = 1)
  }
  
  load(RDataFileIn)
  
  nbClass <- res$mixture$nbCluster
  
  resGetData <-getData(c(dataFile, descFile))
  
  if (nchar(resGetData$warnLog) > 0) # Were there errors when reading the data ?
  {
    cat(resGetData$warnLog,
        file = logFile,
        sep = "",
        append = TRUE)
    quit(save = "no", status = 1)
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
    res <- mixtCompPredict(resGetData$lm,
                           res$variable$param,
                           mcStrategy,
                           nbClass,
                           0.95)
    
    save(res,
         file = RDataFileOut)
    
    writeIDClassHTML(res,
                     IDHTMLFile)
    
    webFolderName <- Sys.getenv("WORKER_WEB_FOLDER")
    if (nchar(webFolderName) > 0) {
      IDJSONFile <- paste(webFolderName,
                          "IDData.json",
                          sep = "/")
      writeIDClassJSON(res,
                       IDJSONFile)
    }
    
    if (nchar(res$mixture$warnLog) > 0)
    {
      cat(res$mixture$warnLog,
          file = logFile,
          sep = "",
          append = TRUE)
      quit(save = "no", status = 1)
    }
    else
    {
      cat("Run completed successfully",
          file = logFile,
          sep = "",
          append = TRUE)
      quit(save = "no", status = 0)
    }
  }
}