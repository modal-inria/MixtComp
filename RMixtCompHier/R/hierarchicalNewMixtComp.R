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
 

# Functions to use the current version of MixtComp with the hierarchical method of Étienne Goffinet
# Author: Quentin Grimonprez (adaptation of Étienne Goffinet's code)

launch_mixtcompNew <- function(dir, nClass, strategy = NULL, mcStrategy = NULL) {

  ## lecture données
  data <- read.table(paste0(dir, "/data_mixtcomp.csv"), sep = ";", header = TRUE, stringsAsFactors = FALSE)
  model <- read.table(paste0(dir, "/descriptor.csv"), sep = ";", header = TRUE, stringsAsFactors = FALSE)
  model = lapply(model, function(x) {list(type = x[1], paramStr = ifelse(length(x) > 1, x[2], "")) })
  nInd <- nrow(data)


  # algo
  if(is.null(mcStrategy)){
    mcStrategy = RMixtCompUtilities:::createAlgo(nbBurnInIter = 15, nbIter = 15,
                                                 nbGibbsBurnInIter = 15, nbGibbsIter = 15, nSemTry = 10,
                                                 nInitPerClass = max(floor(0.1/nClass * nInd), 20),
                                                 ratioStableCriterion = 0.99, nStableCriterion = 10)
  }


  # Check the conditions on the number of Observations min and purity of the categorical variable
  result_strategy = check_strategyNew(strategy, data, model)
  if(result_strategy[[1]] == "error")
    stop(result_strategy[[2]])
  if(result_strategy[[2]] != "")
  {
    print(result_strategy[[2]])
    return(result_strategy[[2]])
  }


  ## run
  res <- JMixtCompLearn(data, model, algo = mcStrategy, nClass = nClass, inputPath = dir, outputFile = paste0(dir, "/mixtcomp_output.json"))

  if(!is.null(res$warnLog))
    print(res$warnLog)

  return(NULL)
}




check_strategyNew <- function(strategy, data, model) {

  if(!is.null(strategy$var)){
    vars_in_resGetData = names(data)

    if(!(strategy$var %in% vars_in_resGetData)){
      return(list("error", paste0("Variable ",strategy$var," indicated in the strategy doesn't exist in the dataset")))
    }

    idx_var = which(strategy$var == vars_in_resGetData)
    var_strategy_type = model[[strategy$var]]$type
    if(var_strategy_type != "Multinomial"){
      return(list("error", paste0("Variable ",strategy$var," is not indicated as Multinomial in the descriptor file")))
    }
    data_var = data[, strategy$var]
    data_var = data_var[which(data_var != "?")]
    print(paste0("length data : ", length(data[, strategy$var])))
    print(paste0("strategy nInd :", strategy$threshold_nInd))
    print(paste0("purity : ", (max(table(data_var) / length(data_var)))))
    print(paste0("strategy purity : ", strategy$threshold_purity))

    if (max(table(data_var) / length(data_var)) > strategy$threshold_purity){return(list("", "Strategy's terms not fulfilled: purity's threshold attained"))}
  }

  nInd <- ifelse(is.matrix(data), nrow(data), length(data[[1]]))

  if(nInd < strategy$threshold_nInd ) {return(list("", "Strategy's terms not fulfilled: not enough individuals"))}

  return(list("", ""))
}


launch_mixtcompNew_predict <- function(param_dir, test_dir, nClass, mcStrategy) {

  ## lecture données
  data <- read.table(paste0(test_dir, "/data_predict.csv"), sep = ";", header = TRUE, stringsAsFactors = FALSE)
  model <- read.table(paste0(param_dir, "/descriptor.csv"), sep = ";", header = TRUE, stringsAsFactors = FALSE)
  model = lapply(model, function(x) {list(type = x[1], paramStr = ifelse(length(x) > 1, x[2], "")) })
  nInd <- nrow(data)


  # algo
  if(is.null(mcStrategy)){
    mcStrategy = RMixtCompUtilities:::createAlgo(nbBurnInIter = 15, nbIter = 15,
                                                 nbGibbsBurnInIter = 15, nbGibbsIter = 15, nSemTry = 10,
                                                 nInitPerClass = max(floor(0.1/nClass * nInd), 20),
                                                 ratioStableCriterion = 0.99, nStableCriterion = 10)
  }


  ###### Mixtcomp run
  res <- RJMixtComp::JMixtCompPredict(data, model, algo = mcStrategy, nClass = nClass, inputPath = test_dir,
                                      paramFile = paste0(param_dir, "/mixtcomp_output.json"), outputFile = paste0(test_dir, "/mixtcomp_output.json"))

  return(NULL)
}
