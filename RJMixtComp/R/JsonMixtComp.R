# MixtComp version 4.0  - july 2019
# Copyright (C) Inria - Université de Lille - CNRS

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
# 
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>

#' @title Learn and predict using RJMixtComp
#' 
#' @description Estimate the parameter of a mixture model or predict the cluster of new samples.
#' 
#' 
#' @param data a data.frame/matrix/list containing the data
#' @param model a list containing the model for each variable. The names of \emph{model}'s element must be in \emph{data}
#' @param algo a list containing the parameters of the SEM-Gibbs algorithm (see \link{createAlgo}).
#' @param nClass the number of class of the mixture model.
#' @param inputPath path of the folder to save the input files.
#' @param outputFile path of the output json file to save.
#' @param paramFile (only for \emph{JMixtCompPredict}) path of the output json file of \emph{JMixtCompLearn} function.
#' 
#' @return A json file and a R list containing 3 lists :
#' \describe{
#'  \item{algo}{a copy of \emph{algo} parameter.}
#'  \item{mixture}{information about the mixture (see \emph{Details}).}
#'  \item{variable}{information about the estimated parameters (see \emph{Details}).}
#' }
#' 
#' @details See external documentation for details about the input/output object of \emph{JMixtCompLearn} and \emph{JMixtCompPredict} functions.
#' 
#' 
#' @examples 
#' \donttest{
#' library(jsonlite)
#' 
#' pathToData <- system.file("extdata", "data.json", package = "RJMixtComp")
#' pathToModel <- system.file("extdata", "desc.json", package = "RJMixtComp")
#' 
#' data <- as.data.frame(fromJSON(pathToData))
#' model <- fromJSON(pathToModel)
#' 
#' algo <- createAlgo()
#' 
#' resLearn <- JMixtCompLearn(data, model, algo = algo, nClass = 2,
#'                            inputPath = ".", outputFile = "reslearn.json")
#' 
#' 
#' file.remove("./algo.json", "./model.json", "./data.json")
#' 
#' resPredict <- JMixtCompPredict(data, model, algo = algo, nClass = 2,
#'                                   inputPath = ".", paramFile = "reslearn.json",
#'                                   outputFile = "respredict.json")
#' 
#' 
#' file.remove("./algo.json", "./model.json", "./data.json", "reslearn.json",
#'             "respredict.json")
#' }
#' 
#' @author Quentin Grimonprez
#' 
#' @export
JMixtCompLearn <- function(data, model, algo = createAlgo(), nClass, inputPath, outputFile)
{
  nInd <- ifelse(is.matrix(data), nrow(data), length(data[[1]]))
  
  algoFile <- paste0(inputPath,"/algo.json")
  write(createAlgoJson(nClass, nInd, algo, mode = "learn"), algoFile)
  
  dataFile <- paste0(inputPath,"/data.json")
  write(createDataJson(data), dataFile)
  
  modelFile <- paste0(inputPath,"/model.json")
  write(createModelJson(model), modelFile)
  
  
  nameExe <- ifelse(Sys.info()["sysname"] == "Windows", "jmc.exe", "jmc")
  pathToJMixtComp <- system.file("exeMixtComp", nameExe, package = "RJMixtComp")
  
  system(paste(pathToJMixtComp, algoFile, dataFile, modelFile, outputFile))
  
  resLearn <- fromJSON(outputFile)
  
  resLearn = convertOutput(resLearn)
  class(resLearn) = "MixtComp"
  
  return(resLearn)
}


#' @rdname JMixtCompLearn
#' @export
JMixtCompPredict <- function(data, model, algo = createAlgo(), nClass, inputPath, paramFile, outputFile)
{
  nInd <- ifelse(is.matrix(data), nrow(data), length(data[[1]]))
  
  algoFile <- paste0(inputPath,"/algo.json")
  write(createAlgoJson(nClass, nInd, algo, mode = "predict"), algoFile)
  
  dataFile <- paste0(inputPath,"/data.json")
  write(createDataJson(data), dataFile)
  
  modelFile <- paste0(inputPath,"/model.json")
  write(createModelJson(model), modelFile)
  
  nameExe <- ifelse(Sys.info()["sysname"] == "Windows", "jmc.exe", "jmc")
  pathToJMixtComp <- system.file("exeMixtComp", nameExe, package = "RJMixtComp")
  
  system(paste(pathToJMixtComp, algoFile, dataFile, modelFile, paramFile, outputFile))
  
  resPredict <- fromJSON(outputFile)
  
  resPredict = convertOutput(resPredict)
  class(resPredict) = "MixtComp"
  
  return(resPredict)
}

