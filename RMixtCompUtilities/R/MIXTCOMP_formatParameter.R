# MixtComp version 4 - july 2019
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
 

# format the model list for rmc function:
# - add paramStr when missing
# - ensure the list format of each element
# @author Quentin Grimonprez
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
# @author Quentin Grimonprez
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


# @author Quentin Grimonprez
completeAlgo <- function(algo)
{
  algoDefault <- createAlgo()
  
  missingNames <- setdiff(names(algoDefault), names(algo))
  
  return(c(algo, algoDefault[missingNames]))
}
