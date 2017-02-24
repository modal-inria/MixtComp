## copie des fonctions de RMixtcomp pour être indépendant du package


# getData -----------------------------------------------------------------
#' @title Read data and descriptor files
#'
#' @description Read csv file of data and descriptor and create an R object for \link{JsonMixtCompCluster} and \link{JsonMixtCompPredict}.
#' 
#' @param ... a vector of two elements : the path to the data file and the path to the descriptor file.
#' 
#' @return a list
#' 
#' @seealso \link{JsonMixtCompCluster} \link{JsonMixtCompPredict}
#' 
#' 
#' @examples 
#' \dontrun{
#' pathToData <- system.file("extdata", "data.csv", package = "RJsonMixtComp")
#' pathToDescriptor <- system.file("extdata", "descriptor.csv", package = "RJsonMixtComp")
#' 
#' res <- getData(c(pathToData, pathToDescriptor))#' 
#' }
#' 
#' @export
getData <- function(...) {
  argList <- list(...)
  lm <- list()
  warnLog = ""
  
  for (k in 1:length(argList)) {
    dataFile <- argList[[k]][1]
    descriptorFile <- argList[[k]][2]
    data <- read.table(file = dataFile,
                       header = TRUE,
                       sep = ";",
                       quote = "\"",
                       dec = ".",
                       fill = FALSE,
                       comment.char = "",
                       stringsAsFactors = FALSE,
                       na.strings = "")
    descriptor <- read.table(file = descriptorFile,
                             header = FALSE,
                             sep = ";",
                             quote = "\"",
                             dec = ".",
                             fill = FALSE,
                             comment.char = "",
                             stringsAsFactors = FALSE)
    for (i in 1:length(descriptor)) { # loop over columns of the descriptor
      currId <- descriptor[1, i]
      currModel <- descriptor[2, i]
      if (dim(descriptor)[1] == 3) {
        param <- descriptor[3, i] # a character string describing the parameter space, that will be parsed directly by the model
      }
      else {
        param <- "" # a zero-sized parameter string will be interpreted by the model as "deduce the parameter space automatically"
      }
      if (! descriptor[1, i] %in% names(data)) { # descriptor does not match data
        warnLog <- paste(warnLog,
                         "Variable ", currId, " asked for in descriptor file ",
                         "but absent from the data file.\n",
                         sep = "")
      }
      else {
        # perform length check and generate named list for current variable
        res <- addVariable(lm,
                           data[[currId]],
                           currId, # id
                           currModel, # model
                           param)
        lm <- res$lm # this discards the warnLog output by addVariable
        warnLog <- paste(warnLog,
                         res$warnLog,
                         sep = "")
      }
    }
  }
  
  return(list(lm = lm,
              warnLog = warnLog))
}



addVariable <- function (lm, data, id, model, paramStr) {
  warnLog = "" # warnLog will contain the various possible error messages
  if (length(lm) == 0) { # first variable is automaticaly added
    lm[[1]] <- list(data = data,
                    model = model,
                    id = id,
                    paramStr = paramStr)
  }
  else if (length(lm[[1]]$data) != length(data)) { # all variable must have the same number of individuals
    warnLog <- paste(warnLog,
                     "Variable ", id, " has ", length(data), " individuals while other variables currently have: ",
                     length(lm[[1]]$data), " individuals. All variables must have the same number of individuals.\n",
                     sep = "")
  }
  else { # variable is fit to be added !
    lm[[length(lm)+1]] <- list(data = data,
                               model = model,
                               id = id,
                               paramStr = paramStr)
  }
  
  return(list(lm = lm,
              warnLog = warnLog))
}

# 
# 
# # getter sortie mixtcomp --------------------------------------------------
# 
# completed <- function(outMixtcomp, with.z_class = FALSE)
# {
#   completedData <- do.call(cbind, lapply(outMixtcomp$variable$data, function(x){x$completed}))
#   
#   if(!with.z_class)
#     completedData = completedData[,-which(colnames(completedData)=="z_class")]
#   
#   return(as.data.frame(completedData))
# }
# 
# 
# 
# getZ_class <- function(outMixtcomp)
# {
#   return(outMixtcomp$variable$data$z_class$completed)
# }
# 
# 
# 
# getType <- function(outMixtcomp, with.z_class = FALSE)
# {
#   type <- unlist(outMixtcomp$variable$type)
#   
#   if(!with.z_class)
#     type = type[-which(names(type)=="z_class")]
#   
#   return(type)
# }
# 
# 
# getParam <- function(varName, outMixtComp)
# {
#  
#   if(varName == "z_class")
#     return(outMixtComp$variable$param$z_class$pi$stat$median)
#    
#   type <- outMixtComp$variable$type[[varName]]
#   param <- switch(type,
#                   "Ordinal" = outMixtComp$variable$param[[varName]]$muPi$stat$median,
#                   "Categorical_pjk" = outMixtComp$variable$param[[varName]]$NumericalParam$stat$median,
#                   "Gaussian_sjk" = outMixtComp$variable$param[[varName]]$NumericalParam$stat$median,
#                   "Poisson_k" = outMixtComp$variable$param[[varName]]$NumericalParam$stat$median)
#   
#   return(param)
# }
# 
# 
# 
# getParamLog <- function(varName, outMixtComp)
# {
#   
#   if(varName == "z_class")
#     return(do.call(rbind, outMixtComp$variable$param$z_class$pi$log))
#   
#   type <- outMixtComp$variable$type[[varName]]
#   param <- switch(type,
#                   "Ordinal" = do.call(rbind, outMixtComp$variable$param[[varName]]$muPi$log),
#                   "Categorical_pjk" = do.call(rbind, outMixtComp$variable$param[[varName]]$NumericalParam$log),
#                   "Gaussian_sjk" = do.call(rbind, outMixtComp$variable$param[[varName]]$NumericalParam$log),
#                   "Poisson_k" = do.call(rbind, outMixtComp$variable$param[[varName]]$NumericalParam$log))
#   
#   return(param)
# }
# 
# 
# getParamName <- function(varName, outMixtComp)
# {
#   
#   if(varName == "z_class")
#     return("proportion")
#   
#   type <- outMixtComp$variable$type[[varName]]
#   param <- switch(type,
#                   "Categorical_pjk" = 1:as.numeric(substr(outMixtComp$variable$param[[varName]]$NumericalParam$paramStr,11,nchar(outMixtComp$variable$param[[varName]]$NumericalParam$paramStr))),
#                   "Gaussian_sjk" = c("mean", "sd"),
#                   "Poisson_k" = "lambda")
#   
#   return(param)
# }
# 
# 
# 
# getBIC <- function(outMixtComp)
# {
#   if(is.null(outMixtComp$mixture$BIC))
#     return(NaN)
#   
#   return(outMixtComp$mixture$BIC)
# }
# 
# getICL <- function(outMixtComp)
# {
#   if(is.null(outMixtComp$mixture$ICL))
#     return(NaN)
#   
#   return(outMixtComp$mixture$ICL)
# }
# 
# 
# getTik <- function(outMixtComp)
# {
#   return(outMixtComp$variable$data$z_class$stat)
# }