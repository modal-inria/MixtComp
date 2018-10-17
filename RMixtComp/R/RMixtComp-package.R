#' @useDynLib RMixtComp
#' @import Rcpp
#' @importFrom stats rbinom rmultinom rnorm rpois runif rweibull rnbinom
#'
#' @title RMixtComp
#' @docType package
#' @aliases RMixtComp-package
#' @name RMixtComp-package
#' @description  
#' Mixture Composer is a project to build mixture models with heterogeneous data
#' sets. The conditional independence property enables composing all the
#' data sets in a single mixture model.
#'
#' 
#' @details
#'
#' Main functions are \link{mixtCompLearn} for clustering, \link{mixtCompPredict} for predict the cluster of new samples with a model learned with \link{mixtCompLearn}. \link{createAlgo} gives you default values for required parameters.
#' 
#' Getters are available to easily access some results (see. \link{mixtCompLearn} for output format): \link{getBIC}, \link{getICL}, \link{getCompletedData}, \link{getParam}, \link{getTik}, \link{getPartition}, \link{getType}, \link{getVarNames}.
#' 
#' 
#' You can compute discriminative power and similiraties with functions: \link{computeDiscrimPowerClass}, \link{computeDiscrimPowerVar}, \link{computeSimilarityClass}, \link{computeSimilarityVar}
#' 
#' All graphics functions are in the \emph{RMixtCompPlot} package. 
#' 
#' 
#' 
#' @examples 
#' \donttest{
#' data(simData)
#'  
#' # define the algorithm's parameters
#' algo <- list(nbBurnInIter = 100,
#'              nbIter = 100,
#'              nbGibbsBurnInIter = 50,
#'              nbGibbsIter = 50,
#'              nInitPerClass = 10,
#'              nSemTry = 20,
#'              confidenceLevel = 0.95)
#' 
#' # run RMixtCompt for clustering
#' resLearn <- mixtCompLearn(simDataLearn$matrix, desc$unsupervised, algo, nbClass = 2:4)
#' 
#' # run RMixtCompt for predicting
#' resPred <- mixtCompPredict(simDataPredict$matrix, desc$unsupervised, algo, resLearn)
#' 
#' }
#' 
#' @seealso \link{mixtCompLearn}
#' 
#' @keywords package
NULL


#' Simulated Heterogeneous data
#'
#' @name simData
#' @aliases simDataLearn simDataPredict simDesc
#' @docType data
#' @keywords data
#' @family data
#' 
#' @usage data(simData)
#' 
#' @format Three object: \emph{simDataLearn}, \emph{simDataPredict} and \emph{simDesc}.
#' 
#' \emph{simDataLearn} is a list containing the data in the three accepted format (list, data.frame and matrix). Data consists of 200 individuals and 9 variables.
#' 
#' \emph{simDataPredict} is a list containing the data in the three accepted format (list, data.frame and matrix). Data consists of 100 individuals and 8 variables.
#' 
#' \emph{simDesc} is a list containing the descriptor lists used for clustering \emph{simDesc$unsupervised} and classification \emph{simDesc$supervised}.
#' 
#' @examples
#' data(simData)
#' str(simDataLearn)
#' str(simDataPredict)
#' str(simDesc)
#'   
NULL


#' Titanic data set
#'
#' @name titanic
#' @docType data
#' @keywords data
#' @family data
#' 
#' @description The data set provides information on the passengers of Titanic. 
#' 
#' @usage data(titanic)
#' 
#' @format A data.frame with 1309 individuals and 8 variables.
#' \itemize{
#'   \item survived: 0 = No, 1 = Yes
#'   \item pclass:	ticket class 	1 = 1st, 2 = 2nd, 3 = 3rd
#'   \item sex: male or female
#'   \item age: age in years
#'   \item sibsp: number of siblings/spouses aboard the Titanic
#'   \item parch: number of parents/children aboard the Titanic 
#'   \item fare: ticket price in pounds
#'   \item embarked: port of Embarkation 	C = Cherbourg, Q = Queenstown, S = Southampton
#' }
#' 
#' @source \url{http://biostat.mc.vanderbilt.edu/wiki/pub/Main/DataSets/titanic3.xls}
#' 
#' @examples
#' \donttest{
#' data(titanic)
#' 
#' head(titanic)
#' 
#' dat <- titanic
#' 
#' # refactor categorical data : survived, sex, embarked
#' dat$sex <- refactorCategorical(dat$sex, c("male", "female", NA), c(1, 2, "?"))
#' dat$embarked <- refactorCategorical(dat$embarked, c("C", "Q", "S", NA), c(1, 2, 3, "?"))
#' dat$survived <- refactorCategorical(dat$survived, c(0, 1, NA), c(1, 2, "?"))
#' 
#' # replace all NA by ?
#' dat[is.na(dat)] = "?" 
#' 
#' # create descriptor
#' desc <- list(pclass = "Poisson",
#'              survived = "Multinomial",
#'              sex = "Multinomial",
#'              age = "Gaussian",
#'              sibsp = "Poisson",
#'              parch = "Poisson",
#'              fare = "Gaussian",
#'              embarked = "Multinomial")
#' 
#' # create algo
#' algo <- createAlgo()
#' 
#' # run clustering
#' resLearn <- mixtCompLearn(dat, desc, algo, nClass = 2:15, criterion = "ICL", nRun = 5)
#' 
#' summary(resLearn)
#' 
#' }
#'   
NULL