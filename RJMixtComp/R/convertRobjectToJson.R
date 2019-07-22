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


# @author Quentin Grimonprez
createAlgoJson <- function(nClass, nInd, mcStrategy, mode)
{
  toKeep <- names(mcStrategy) %in% names(createAlgo())
  unrequiredFields = mcStrategy[!toKeep]
  
  algo = c(list(nClass = nClass, nInd = nInd), RMixtCompUtilities:::completeAlgo(mcStrategy[toKeep]),
           list(mode = mode), unrequiredFields)
  
  return(toJSON(algo, auto_unbox = TRUE, pretty = TRUE))
}

# @param data a data.frame where each column corresponds to a variable 
# @author Quentin Grimonprez
createDataJson <- function(data)
{
  data = RMixtCompUtilities:::formatData(data)
  toJSON(data, auto_unbox = FALSE, pretty = TRUE)
}



# @param descriptor a data.frame where each column corresponds to a variable. Each column contains the model and eventually supplementary parameters
# @author Quentin Grimonprez
createModelJson <- function(model)
{
  model <- RMixtCompUtilities:::formatModel(model)
  toJSON(model, auto_unbox = TRUE, pretty = TRUE)
}