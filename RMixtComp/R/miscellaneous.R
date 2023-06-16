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


#' Extract a MixtComp object
#'
#' Extract a MixtComp object from a MixtCompLearn object
#'
#' @param object \link{mixtCompLearn} output
#' @param K number of classes of the model to extract
#'
#' @return a \code{MixtComp} object containing the clustering model with K classes
#'
#' @examples
#' # run clustering
#' resLearn <- mixtCompLearn(data.frame(x = rnorm(500)),
#'   nClass = 1:3, criterion = "ICL",
#'   nRun = 1, nCore = 1
#' )
#'
#' # extract the model with 2 classes
#' clustModel <- extractMixtCompObject(resLearn, K = 2)
#'
#' @author Quentin Grimonprez
#'
#' @export
extractMixtCompObject <- function(object, K) {
  if (!("MixtCompLearn" %in% class(object))) {
    stop("object must be a MixtCompLearn object.")
  }

  if (missing(K)) {
    K <- object$algo$nClass
  }

  if (length(K) > 1) {
    stop("K must be an integer.")
  }

  ind <- which(object$nClass == K)

  if (length(ind) == 0) {
    stop("K is not a value from object$nClass.")
  }

  return(object$res[[ind]])
}
