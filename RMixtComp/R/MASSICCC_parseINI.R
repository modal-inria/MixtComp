parseINI <- function(iniFile) {
  paramList <- list()
  iniContent <- read.table(iniFile,
                           sep = "=",
                           strip.white = TRUE,
                           stringsAsFactors = FALSE)
  
  nbClass <- iniContent[which(iniContent[, 1] == "nbCluster"), 2]
  parameterEdgeAuthorized <- iniContent[which(iniContent[, 1] == "parameterEdgeAuthorized"), 2]
  
  if (length(nbClass) == 1) {
    paramList$nbClass <- as.integer(nbClass)
  } else {
    stop("param.ini, nbCluster not defined correctly")
  }
  
  if (length(parameterEdgeAuthorized) == 0) {
    paramList$parameterEdgeAuthorized <- FALSE
  } else if (length(parameterEdgeAuthorized) == 1) {
    if (grepl("TRUE", parameterEdgeAuthorized)) {
      paramList$parameterEdgeAuthorized <- TRUE
    } else if (grepl("FALSE", parameterEdgeAuthorized)) {
      paramList$parameterEdgeAuthorized <- FALSE
    } else {
      stop("param.ini, parameterEdgeAuthorized not defined correctly")
    }
  } else {
    stop("param.ini, parameterEdgeAuthorized not defined correctly")
  }
  
  return(paramList)
}
