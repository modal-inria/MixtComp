webDemoLearn <- function(folderName) {
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
  if (nchar(checkDimLog) > 0) {
    cat(checkDimLog,
        file = logFile,
        sep = "",
        append = TRUE)
    quit(save = "no", status = 1)
  }
  
  paramList <- parseINI(iniFile)
  
  if (paramList$nbClass > 30) {
    cat("The maximum number of classes in the demo version of MixtComp on BigStat is 30.",
        file = logFile,
        sep = "",
        append = TRUE)
    quit(save = "no", status = 1)
  }
  
  resGetData <- getData(c(dataFile, descFile))
  
  if (nchar(resGetData$warnLog) > 0) { # Were there errors when reading the data ?
    cat(resGetData$warnLog,
        file = logFile,
        sep = "",
        append = TRUE)
    quit(save = "no", status = 1)
  }
  else { # run can be carried out
    # creation of strategy list
    mcStrategy <- list(nbTrialInInit = 2,
                       nbBurnInIter = 100,
                       nbIter = 100,
                       nbGibbsBurnInIter = 100,
                       nbGibbsIter = 100,
                       parameterEdgeAuthorized = paramList$parameterEdgeAuthorized)
    
    # launch the MixtComp algorithm
    res <- mixtCompCluster(resGetData$lm,
                           mcStrategy,
                           paramList$nbClass,
                           0.95)
    
    save(res,
         file = RDataFileOut)
    
    if (nchar(res$mixture$warnLog) == 0) {
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
      
      cat("Run completed successfully",
          file = logFile,
          sep = "",
          append = TRUE)
      quit(save = "no", status = 0)
    }
    else {
      cat(res$mixture$warnLog,
          file = logFile,
          sep = "",
          append = TRUE)
      quit(save = "no", status = 1)
    }
  }
}