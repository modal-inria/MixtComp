webDemoLearn <- function(folderName)
{
  iniContent <- read.table(paste(folderName,
                                 "in/param.ini",
                                 sep = "/"),
                           sep = "=",
                           strip.white = TRUE,
                           stringsAsFactors = FALSE)
  
  nbClass = iniContent[which(iniContent[, 1] == "nbCluster"), 2]
  
  lm <- getData(c(paste(folderName,
                        "in/data.csv",
                        sep = "/"),
                  paste(folderName,
                        "in/descriptor.csv",
                        sep = "/")))
  
  # creation of strategy list
  mcStrategy <- list(nbTrialInInit = 2,
                     nbBurnInIter = 100,
                     nbIter = 100,
                     nbGibbsBurnInIter = 100,
                     nbGibbsIter = 100)
  
  # launch of the MixtComp algorithm
  res <- mixtCompCluster(lm,
                         mcStrategy,
                         nbClass,
                         0.95)
  
  fileConn <- file(paste(folderName,
                         "out/log.txt",
                         sep = "/"))
  if (nchar(res$mixture$warnLog) > 0)
  {
    cat(res$mixture$warnLog,
        file = fileConn,
        sep = "")
  }
  else
  {
    cat("Run completed successfully",
        file = fileConn,
        sep = "")
  }
  close(fileConn)
  
  save(res,
       file = paste(folderName,
                    "out/output.RData",
                    sep = "/"))
}