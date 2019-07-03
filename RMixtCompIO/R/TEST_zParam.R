# MixtComp version 4.0  - july 2019
# Copyright (C) Inria - Université de Lille - CNRS

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
