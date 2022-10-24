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


# input the model of variables of the data (given in a list or data.frame format)
# model are imputed among "Gaussian" (numeric data), "Multinomial" (character or factor) and "Poisson" (integer).
# @author Quentin Grimonprez
imputModel <- function(data) {
  if (is.matrix(data)) {
    stop(paste(
      "Can imput model only with data in data.frame or list format.",
      "Please provide data in these formats or provide a model parameter."
    ))
  }

  varNames <- names(data)
  index <- seq_along(data)
  model <- lapply(index, function(i) list(type = imputModelIntern(data[[i]], varNames[i]), paramStr = ""))
  names(model) <- varNames

  # check there is no variable named z_class imputed with something else than "LatentClass"
  if (any(names(model) == "z_class")) {
    if (model$z_class$type != "LatentClass") {
      model$z_class <- NULL # we remove the variable
      warning(paste(
        "A variable named z_class was provided but not with the right type (numeric instead of integer or factor).",
        "The z_class name is reserved for the latent class variable.",
        "This variable was removed from the model.",
        "If you want to keep it, change the type of the variable to integer or factor",
        "(if this variable is the latent class variable) or change the variable name."
      ))
    }
  }

  return(model)
}

# imput the model of a given variable
# model are imputed among "Gaussian" (numeric data), "Multinomial" (character or factor) and "Poisson" (integer).
# @author Quentin Grimonprez
imputModelIntern <- function(variable, name) {
  switch(class(variable),
    "numeric" = "Gaussian",
    "integer" = ifelse(name == "z_class", "LatentClass", "Poisson"),
    "factor" = ifelse(name == "z_class", "LatentClass", "Multinomial"),
    "character" = ifelse(name == "z_class", "LatentClass", "Multinomial"),
    stop(paste0("Cannot impute the model for variable ", name, ". Please provide the model parameter."))
  )
}

# add default hyperparameters
# @author Quentin Grimonprez
completeModel <- function(model, data = NULL) {
  for (nameVar in names(model))
  {
    if ((model[[nameVar]]$type %in% c("Func_CS", "Func_SharedAlpha_CS")) && model[[nameVar]]$paramStr == "") {
      timeVector <- convertFunctionalToVector(data[[nameVar]][sample(length(data[[nameVar]]), 1)])$time
      nSub <- max(min(floor(length(timeVector) / 50), 15), 1)
      model[[nameVar]]$paramStr <- paste0("nSub: ", nSub, ", nCoeff: 2")

      warning(paste0(
        "No hyperparameters given for functional variable ", nameVar,
        ". Use the following default values: \"nSub: ", nSub, ", nCoeff: 2\" for paramStr."
      ))
    }
  }

  return(model)
}



# in basic mode, data is a data.frame or a matrix
# @author Quentin Grimonprez
formatDataBasicMode <- function(data, model, dictionary = NULL) {
  createDictionary <- is.null(dictionary)
  if (createDictionary) {
    dictionary <- list()
  }

  data <- as.list(data)

  for (name in names(data)) {
    if (model[[name]]$type %in% c("Multinomial", "LatentClass")) {
      if (!is.integer(data[[name]])) { # z_class is given in integer : no need to use the dictionary
        if (createDictionary) {
          oldCateg <- sort(unique(data[[name]]))
          oldCateg <- oldCateg[!is.na(oldCateg)]
          dictionary[[name]] <- list(old = as.character(oldCateg), new = as.character(seq_along(oldCateg)))
        } else {
          if (!(name %in% names(dictionary))) {
            stop(paste0("No dictionary given for variable ", name))
          }
        }

        data[[name]] <- refactorCategorical(data[[name]], dictionary[[name]]$old, newCateg = dictionary[[name]]$new)
      }
    }

    data[[name]] <- as.character(data[[name]])
    data[[name]][is.na(data[[name]])] <- "?"
  }

  return(list(data = data, dictionary = dictionary))
}

# check the number of class given by the user in mixtCompPredict
# @author Quentin Grimonprez
checkNClass <- function(nClass, resLearn) {
  if (is.null(nClass)) {
    nClass <- resLearn$algo$nClass
  }

  if (length(nClass) > 1) {
    nClass <- nClass[1]
    warning("Several nClass given. Only the first is used.")
  }

  if ("MixtCompLearn" %in% class(resLearn)) {
    if (!(nClass %in% resLearn$nClass)) {
      nClass <- resLearn$algo$nClass
      warning(paste(
        "The provided value of nClass does not match any results in the resLearn object.",
        "nClass is set to the value with the best criterion value."
      ))
    }
  } else {
    if (nClass != resLearn$algo$nClass) {
      nClass <- resLearn$algo$nClass
      warning(paste(
        "The provided value of nClass does not match the value in the resLearn object.",
        "nClass is set to the value of the resLearn object."
      ))
    }
  }

  return(nClass)
}

# @author Quentin Grimonprez
changeClassName <- function(rowNames, dictionary) {
  for (i in seq_along(rowNames))
  {
    nameSplit <- strsplit(rowNames[i], split = ",")[[1]]

    nameSplit2 <- strsplit(nameSplit[1], ": ")[[1]]
    nameSplit2[2] <- dictionary$z_class$old[dictionary$z_class$new == nameSplit2[2]]
    nameSplit[1] <- paste(nameSplit2[1], nameSplit2[2], sep = ": ")

    rowNames[i] <- paste(nameSplit, collapse = ",")
  }

  return(rowNames)
}


# in basic mode add dictionaries of categories for Multinomial ans LatenClass models
# change names in param, log...
# @author Quentin Grimonprez
formatOutputBasicMode <- function(res, dictionary) {
  res$algo$dictionary <- dictionary

  for (varName in setdiff(names(res$algo$dictionary), "z_class")) {
    res$variable$data[[varName]]$completed <- refactorCategorical(
      res$variable$data[[varName]]$completed,
      res$algo$dictionary[[varName]]$new, res$algo$dictionary[[varName]]$old
    )

    res$variable$data[[varName]]$stat <- lapply(res$variable$data[[varName]]$stat, function(x) {
      out <- as.data.frame(x)
      out[, 1] <- refactorCategorical(out[, 1], res$algo$dictionary[[varName]]$new, res$algo$dictionary[[varName]]$old)
      rownames(out) <- NULL
      return(out)
    })

    rownames(res$variable$param[[varName]]$stat) <- paste0(
      gsub("[0-9]*$", "", rownames(res$variable$param[[varName]]$stat)), res$algo$dictionary[[varName]]$old
    )
    rownames(res$variable$param[[varName]]$log) <- rownames(res$variable$param[[varName]]$stat)
  }

  # this will not work for non simple model. It is not a problem because in basic mode only simple models are considered
  if ("z_class" %in% names(res$algo$dictionary)) {
    varNames <- getVarNames(res, with.z_class = TRUE)

    res$variable$data$z_class$completed <- refactorCategorical(
      res$variable$data$z_class$completed, res$algo$dictionary$z_class$new, res$algo$dictionary$z_class$old
    )
    colnames(res$variable$data$z_class$stat) <- changeClassName(colnames(res$variable$data$z_class$stat), res$algo$dictionary)

    for (varName in varNames) {
      rowNames <- rownames(res$variable$param[[varName]]$stat)
      rowNames <- changeClassName(rowNames, res$algo$dictionary)
      rownames(res$variable$param[[varName]]$stat) <- rownames(res$variable$param[[varName]]$log) <- rowNames
    }
  }

  return(res)
}
