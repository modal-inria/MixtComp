webDemoLearn <- function(folderName)
{
  iniFile <- paste(folderName,
                   "in/param.ini",
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
  
  checkDimLog <- validDim(dataFile,
                          maxIndividual,
                          maxVariable)
  if (nchar(checkDimLog) > 0)
  {
    cat(checkDimLog,
        file = logFile,
        sep = "",
        append = TRUE)
    quit(save = "no", status = 1)
  }
  
  iniContent <- read.table(iniFile,
                           sep = "=",
                           strip.white = TRUE,
                           stringsAsFactors = FALSE)
  
  nbClass = iniContent[which(iniContent[, 1] == "nbCluster"), 2]
  
  resGetData <- getData(c(dataFile, descFile))
  
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
    res <- mixtCompCluster(resGetData$lm,
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