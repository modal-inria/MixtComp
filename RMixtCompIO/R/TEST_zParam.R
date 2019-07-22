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
 

# This file contains functions to create the parameter object in the different examples and test of the package
# @author Vincent Kubicki
zParam <- function() {
  z_class <- list() # z_class must be the first variable in the list
  
  z_class$name <- "z_class"
  z_class$type <- "LatentClass"
  z_class$paramStr <- ""
  z_class$param <- c(0.3, 0.7) # parameters for z_class are the mixture proportions
  #  z_class$param <- c(1.) # parameters for z_class are the mixture proportions
  z_class$allPresent <- FALSE
  z_class$allMissing <- TRUE
  z_class$fixedInitialization <- FALSE
  
  return(z_class)
}

# @author Vincent Kubicki
zParamFixedInit <- function() {
  z_class <- list() # z_class must be the first variable in the list
  
  z_class$name <- "z_class"
  z_class$type <- "LatentClass"
  z_class$paramStr <- ""
  z_class$param <- c(0.3, 0.7) # parameters for z_class are the mixture proportions
  #  z_class$param <- c(1.) # parameters for z_class are the mixture proportions
  z_class$allPresent <- TRUE
  z_class$allMissing <- FALSE
  z_class$fixedInitialization <- TRUE
  z_class$paramStr <- "fixedInitialization"
  
  return(z_class)
}
