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
    
    if(!is.list(dataFile))
    {
      data <- read.table(file = dataFile,
                         header = TRUE,
                         sep = ";",
                         quote = "\"",
                         dec = ".",
                         fill = FALSE,
                         comment.char = "",
                         stringsAsFactors = FALSE,
                         na.strings = "")
    }else{
      data <- dataFile[[1]]
    }
    
    
    if(!is.list(descriptorFile))
    {
      descriptor <- read.table(file = descriptorFile,
                               header = TRUE,
                               sep = ";",
                               quote = "\"",
                               dec = ".",
                               fill = FALSE,
                               comment.char = "",
                               stringsAsFactors = FALSE)
    }else{
      descriptor <- descriptorFile[[1]]
    }
    
    nameDesc <- colnames(descriptor)
    
    for (i in 1:length(nameDesc)) { # loop over columns of the descriptor
      currId <- nameDesc[i]
      currModel <- descriptor[1, i]
      if (dim(descriptor)[1] == 2) {
        param <- descriptor[2, i] # a character string describing the parameter space, that will be parsed directly by the model
      }
      else {
        param <- "" # a zero-sized parameter string will be interpreted by the model as "deduce the parameter space automatically"
      }
      if (! (currId %in% colnames(data))) { # descriptor does not match data
        warnLog <- paste(warnLog,
                         "Variable ", currId, " asked for in descriptor file ",
                         "but absent from the data file.\n",
                         sep = "")
      }
      else {
        # perform length check and generate named list for current variable
        res <- addVariable(lm,
                           data[,currId],
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

addVariable <- function (lm,
                         data,
                         id,
                         model,
                         paramStr) {
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


# getter sortie mixtcomp --------------------------------------------------
#' @title Get the completed data from MixtComp object
#'
#' @description Get the completed data from MixtComp object
#'
#' @param outMixtComp output object of \link{JsonMixtCompCluster} or \link{JsonMixtCompPredict} functions.
#' @param with.z_class if TRUE, z_class is returned with the data.
#'
#' @return  a matrix with the data completed by MixtComp (z_class is in the first column and then variables are sorted in alphabetic order, it may differ from the original order of the data).
#'
#' 
#' @export
getCompletedData <- function(outMixtComp, with.z_class = FALSE)
{
  completedData <- do.call(cbind, lapply(outMixtComp$variable$data, function(x){x$completed}))
  
  if(!with.z_class)
    completedData = completedData[,-which(colnames(completedData)=="z_class"), drop = FALSE]
  
  return(as.data.frame(completedData))
}



#' @title Get the estimated class from MixtComp object
#'
#' @description Get the estimated class from MixtComp object
#'
#' @param outMixtComp output object of \link{JsonMixtCompCluster} or \link{JsonMixtCompPredict} functions.
#'
#' @return a vector containing the estimated class for each individual.
#' 
#' @export
getZ_class <- function(outMixtComp)
{
  return(outMixtComp$variable$data$z_class$completed)
}



#' @title Get the type of model for each variable
#'
#' @description Get the type of model for each variable
#'
#' @param outMixtComp output object of \link{JsonMixtCompCluster} or \link{JsonMixtCompPredict} functions.
#' @param with.z_class if TRUE, the type of z_class is returned.
#'
#' @return a vector containing the type of models associated with each individual.
#'
#' 
#' @export
getType <- function(outMixtComp, with.z_class = FALSE)
{
  type <- unlist(outMixtComp$variable$type)
  
  if(!with.z_class)
    type = type[-which(names(type)=="z_class")]
  
  return(type)
}


#' @title Get the empiric tik
#'
#' @description Get the empiric tik
#'
#' @param outMixtComp output object of \link{JsonMixtCompCluster} or \link{JsonMixtCompPredict} functions.
#'
#' @return a matrix containing the tik for each individuals (in row) and each class (in column).
#' 
#' @export
getTik <- function(outMixtComp)
{
  return(outMixtComp$variable$data$z_class$stat)
}



#' @title Get the estimated parameter
#'
#' @description Get the estimated parameter
#'
#' @param varName name of the variable to get parameter
#' @param outMixtComp output object of \link{JsonMixtCompCluster} or \link{JsonMixtCompPredict}  functions.
#'
#' @return the parameter of the variable
#'
#' 
#' @export
getParam <- function(varName, outMixtComp)
{
  
  if(varName == "z_class")
    return(outMixtComp$variable$param$z_class$pi$stat$median)
  
  type <- outMixtComp$variable$type[[varName]]
  param <- switch(type,
                  "Ordinal" = outMixtComp$variable$param[[varName]]$muPi$stat[,1],
                  "Categorical_pjk" = outMixtComp$variable$param[[varName]]$NumericalParam$stat[,1],
                  "Gaussian_sjk" = outMixtComp$variable$param[[varName]]$NumericalParam$stat[,1],
                  "Poisson_k" = outMixtComp$variable$param[[varName]]$NumericalParam$stat[,1],
                  "Rank" = list(pi = outMixtComp$variable$param[[varName]]$pi$stat, mu = outMixtComp$variable$param[[varName]]$mu$stat),
                  "Functional" = list(alpha = outMixtComp$variable$param[[varName]]$alpha$stat[,1],
                                      beta = outMixtComp$variable$param[[varName]]$beta$stat[,1],
                                      sd = outMixtComp$variable$param[[varName]]$sd$stat[,1]))
  
  return(param)
}


#' @name getBIC
#' @aliases getICL
#' 
#' @usage getBIC(outMixtComp)
#' getICL(outMixtComp)
#' 
#' @title Get criterion value
#'
#' @description Get criterion value
#'
#' @param outMixtComp output object of \link{JsonMixtCompCluster} or \link{JsonMixtCompPredict} functions.
#'
#' @return value of the criterion
#'
#' @export
getBIC <- function(outMixtComp)
{
  if(is.null(outMixtComp$mixture$BIC))
    return(NaN)
  
  return(outMixtComp$mixture$BIC)
}

#' @export
getICL <- function(outMixtComp)
{
  if(is.null(outMixtComp$mixture$ICL))
    return(NaN)
  
  return(outMixtComp$mixture$ICL)
}


#' @title Convert an R object to json
#'
#' @description convert an R object to a json file to use with \link{JsonMixtCompPredict} function
#'
#' @param outMixtComp output object of \link{JsonMixtCompCluster} or \link{JsonMixtCompPredict}  functions.
#' @param filename name of the variable to get parameter
#'
#' @export
writeJsonParamFile <- function(outMixtComp, filename)
{
  
  # TODO ne marche qu'avec du gaussien
  res2 <- outMixtComp
  res2$mixture = NULL
  res2$strategy = NULL
  res2$variable$param$z_class$pi$log = NULL
  res2$variable$data = NULL
  
  varName <- setdiff(names(outMixtComp$variable$type), "z_class")
  
  res2$variable$param$z_class$pi$stat <- list(median = outMixtComp$variable$param$z_class$pi$stat[,1],
                                              "q 2.5%" = outMixtComp$variable$param$z_class$pi$stat[,2],
                                              "q 97.5%" = outMixtComp$variable$param$z_class$pi$stat[,3])
  
  
  for(nom in varName)
  {
    res2$variable$param[[nom]]$NumericalParam$stat <- list(median = outMixtComp$variable$param[[nom]]$NumericalParam$stat[,1],
                                                           "q 2.5%" = outMixtComp$variable$param[[nom]]$NumericalParam$stat[,2],
                                                           "q 97.5%" = outMixtComp$variable$param[[nom]]$NumericalParam$stat[,3])
    
  }
  
  
  write(x = jsonlite::toJSON(res2, auto_unbox = TRUE), filename)
  
}
