
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
#' # create model
#' model <- list(pclass = "Poisson",
#'               survived = "Multinomial",
#'               sex = "Multinomial",
#'               age = "Gaussian",
#'               sibsp = "Poisson",
#'               parch = "Poisson",
#'               fare = "Gaussian",
#'               embarked = "Multinomial")
#' 
#' # create algo
#' algo <- createAlgo()
#' 
#' # run clustering
#' resLearn <- mixtCompLearn(dat, model, algo, nClass = 2:15, criterion = "ICL", nRun = 5)
#' 
#' summary(resLearn)
#' 
#' }
#'   
NULL

#' Canadian average annual weather cycle
#'
#' @name CanadianWeather
#' @docType data
#' @keywords data
#' @family data
#' 
#' @description Daily temperature and precipitation at 35 different locations in Canada averaged over 1960 to 1994.
#' 
#' @usage data(CanadianWeather)
#' 
#' @format A list containing 3 elements:
#' \itemize{
#'   \item tempav: a matrix of dimensions (365, 35) giving the average temperature in degrees celcius for each day of the year.
#'   \item precav: a matrix of dimensions (365, 35) giving the average rainfall in millimeters for each day of the year.
#'   \item time: sequence from 1 to 365
#' }
#' 
#' @source 
#' Ramsay, James O., and Silverman, Bernard W. (2006), Functional Data Analysis, 2nd ed., Springer, New York.
#'
#' Ramsay, James O., and Silverman, Bernard W. (2002), Applied Functional Data Analysis, Springer, New York
#' 
#' 
#' @examples
#' \donttest{
#' data(CanadianWeather)
#' 
#' # convert functional to MixtComp format
#' dat <- list(tempav = apply(CanadianWeather$tempav, 2, 
#'                            function(x) createFunctional(CanadianWeather$time, x)), 
#'             precav = apply(CanadianWeather$precav, 2, 
#'                            function(x) createFunctional(CanadianWeather$time, x)))
#' 
#' # create model with 3 subregressions ans 2 coefficients per regression
#' model <- list(tempav = list(type = "Func_CS", paramStr = "nSub: 3, nCoeff: 2"),
#'               precav = list(type = "Func_CS", paramStr = "nSub: 3, nCoeff: 2"))
#' 
#' # create algo
#' algo <- createAlgo()
#' 
#' # run clustering
#' resLearn <- mixtCompLearn(dat, model, algo, nClass = 2:4, criterion = "ICL", nRun = 3)
#' 
#' summary(resLearn)
#' 
#' getPartition(resLearn)
#' getTik(resLearn, log = FALSE)
#' 
#' }
#'   
NULL