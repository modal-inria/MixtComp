# input the model of variables of the data (given in a list or data.frame format)
# model are imputed among "Gaussian" (numeric data), "Multinomial" (character or factor) and "Poisson" (integer). 
imputModel <- function(data)
{
  if(is.matrix(data))
    stop("Can imput model only with data in data.frame or list format. Please provide data in these formats of provide a model parameter.")   
  
  if(is.data.frame(data))
  {
    varNames <- colnames(data)
    index <- 1:ncol(data)
    model <- lapply(index, function(i) list(type = imputModelIntern(data[,i], varNames[i]), paramStr = ""))
    names(model) = varNames
  }else{
    varNames <- names(data)
    index <- seq_along(data)
    model <- lapply(index, function(i) list(type = imputModelIntern(data[[i]], varNames[i]), paramStr = ""))
    names(model) = varNames
  }
  
  return(model)
}

# imput the model of a given variable
# model are imputed among "Gaussian" (numeric data), "Multinomial" (character or factor) and "Poisson" (integer). 
imputModelIntern <- function(variable, name)
{
  switch(class(variable),
         "numeric" = "Gaussian",
         "integer" = ifelse(name == "z_class", "LatentClass", "Poisson"),
         "factor" = "Multinomial",
         "character" = "Multinomial",
         stop(paste0("Cannot impute the model for variable ", name,". Please provide the model parameter.")))
}

# format the model list for rmc function:
# - add paramStr when missing
# - ensure the list format of each element
formatModel <- function(model)
{
  model = lapply(model, function(x){
    if(!is.list(x))
      x = list(type = x)
    if(!("paramStr" %in% names(x)))
      x$paramStr = ""
    
    return(x)
  })
  
  return(model)
}


# format data.frame or matrix in list of character
# keep list in list format
formatData <- function(data)
{
  if(is.data.frame(data) || is.matrix(data))
  {
    out <- lapply(1:ncol(data), function(i) as.character(data[,i]))
    names(out) = colnames(data)
  }else{
    out <- lapply(data, as.character)
  }
  
  return(out)
}


# in basic mode, data is a data.frame or a matrix
formatDataBasicMode <- function(data, model, dictionary = NULL)
{
  createDictionary <- is.null(dictionary)
  if(createDictionary)
    dictionary = list()
  
  data = as.list(data)

  for(name in names(data))
  {
    if(model[[name]]$type == "Multinomial")
    {
      if(createDictionary)
      {
        oldCateg <- unique(data[[name]])
        oldCateg = oldCateg[!is.na(oldCateg)]
        dictionary[[name]] = list(old = as.character(oldCateg), new = as.character(seq_along(oldCateg)))
      }else{
        if(!(name %in% names(dictionary)))
          stop(paste0("No dictionary given for variable ", name))
      }

      data[[name]] = refactorCategorical(data[[name]], dictionary[[name]]$old, newCateg = dictionary[[name]]$new)
    }

    data[[name]] = as.character(data[[name]])
    data[[name]][is.na(data[[name]])] = "?"
  }
    
  return(list(data = data, dictionary = dictionary))  
}

# check the number of class given by the user in mixtCompPredict
checkNClass <- function(nClass, resLearn)
{
  if(is.null(nClass))
    nClass = resLearn$algo$nClass
  
  if(length(nClass) > 1)
  {
    nClass = nClass[1]
    warning("Several nClass given. Only the first is used.")
  }
  
  if("MixtCompLearn" %in% class(resLearn))
  {
    if(!(nClass %in% resLearn$nClass))
    {
      nClass = resLearn$algo$nClass
      warning("The provided value of nClass does not match any results in the resLearn object. nClass is set to the value with the best criterion value.")
    }
  }else{
    if(nClass != resLearn$algo$nClass)
    {
      nClass = resLearn$algo$nClass
      warning("The provided value of nClass does not match the value in the resLearn object. nClass is set to the value of the resLearn object.")
    }
  }
  
  return(nClass)
}

completeAlgo <- function(algo)
{
  algoDefault <- createAlgo()
  
  missingNames <- setdiff(names(algoDefault), names(algo))
  
  return(c(algo, algoDefault[missingNames]))
}