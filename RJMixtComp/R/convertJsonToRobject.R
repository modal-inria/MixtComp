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

# @param x result from MixtComp read with fromJSON
# @author Quentin Grimonprez
convertOutput <- function(x)
{
  x = lapply(x, convertJsonElement)
  if(any(sapply(x, is.list)))
  {
    x[sapply(x, is.list)] = lapply(x[sapply(x, is.list)], convertOutput)
  }
  
  return(x)
}

# @author Quentin Grimonprez
convertJsonElement <- function(x)
{
  if("ctype" %in% names(x))
  {
    x <- switch(x$ctype,
                "Matrix" = {
                  if((x$nrow == 0) | (x$ncol == 0))
                    return(matrix(nrow = x$nrow, ncol = x$ncol, dimnames = list(x$rowNames, x$colNames)))
                  
                  rownames(x$data) = x$rowNames
                  colnames(x$data) = x$colNames
                  
                  return(x$data)
                }, 
                "Vector" = {
                  if(x$nrow == 0)
                    return(c())
                  if(length(x$rowNames) != 0) 
                    names(x$data) = x$rowNames
                  
                  return(x$data)
                })
  }
  return(x)
}
