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
 

#' Simulated Heterogeneous data
#'
#' @name simData
#' @aliases simData
#' @docType data
#' @keywords data
#' @family data
#' 
#' @usage data(simData)
#' 
#' @format A list containing three elements: \emph{dataLearn}, \emph{dataPredict} and \emph{model}.
#' \itemize{
#'   \item \emph{dataLearn} is a list containing the data in the three accepted format (list, data.frame and matrix). Data consists of 200 individuals and 9 variables.
#'   \item \emph{dataPredict} is a list containing the data in the three accepted format (list, data.frame and matrix). Data consists of 100 individuals and 8 variables.
#'   \item \emph{model} is a list containing the model lists used for clustering \emph{model$unsupervised} and classification \emph{model$supervised}.
#' }
#' 
#' 
#' @examples
#' data(simData)
#' str(simData)
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
#'   \item survived: 0 = No, 1 = Yes (factor)
#'   \item pclass:	ticket class 	1st, 2nd, 3rd (factor)
#'   \item sex: male or female (factor)
#'   \item age: age in years
#'   \item sibsp: number of siblings/spouses aboard the Titanic
#'   \item parch: number of parents/children aboard the Titanic 
#'   \item fare: ticket price in pounds
#'   \item embarked: port of Embarkation 	C = Cherbourg, Q = Queenstown, S = Southampton (factor)
#' }
#' 
#' @source Titanic People Database, Encyclopedia Titanica, \url{https://www.encyclopedia-titanica.org/titanic-survivors/}
#' 
#' \url{https://www.kaggle.com/c/titanic/data}
#' 
#' @examples
#' \donttest{
#' data(titanic)
#' 
#' head(titanic)
#' 
#' ## Use the MixtComp format
#' dat <- titanic
#' 
#' # refactor categorical data: survived, sex, embarked and pclass
#' dat$sex <- refactorCategorical(dat$sex, c("male", "female", NA), c(1, 2, "?"))
#' dat$embarked <- refactorCategorical(dat$embarked, c("C", "Q", "S", NA), c(1, 2, 3, "?"))
#' dat$survived <- refactorCategorical(dat$survived, c(0, 1, NA), c(1, 2, "?"))
#' dat$pclass <- refactorCategorical(dat$pclass, c("1st", "2nd", "3rd"), c(1, 2, 3))
#' 
#' # replace all NA by ?
#' dat[is.na(dat)] = "?" 
#' 
#' # create model
#' model <- list(pclass = "Multinomial",
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
#' resLearn <- mixtCompLearn(dat, model, algo, nClass = 2:15, criterion = "ICL", nRun = 3, nCore = 1)
#' summary(resLearn)
#' 
#' plot(resLearn) 
#' 
#' 
#' ## Use standard data.frame and R format because titanic contains only standard variables.
#' # mixtCompLearn in "basic" mode without model parameters and data as a data.frame.
#' # A Multinomial model is used for factor variables, a Poisson for integer
#' # and a Gaussian for numeric.
#' resLearn <- mixtCompLearn(titanic, nClass = 2:15, nRun = 3, nCore = 1)
#' 
#' # imputed model
#' getType(resLearn)
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
#' @description Daily temperature and precipitation at 35 different locations in Canada averaged over 1960 to 1994. Data from \code{fda} package.
#' 
#' @usage data(CanadianWeather)
#' 
#' @format A list containing 5 elements:
#' \itemize{
#'   \item tempav: a matrix of dimensions (365, 35) giving the average temperature in degrees celcius for each day of the year.
#'   \item precav: a matrix of dimensions (365, 35) giving the average rainfall in millimeters for each day of the year.
#'   \item time: sequence from 1 to 365.
#'   \item coordinates: a matrix giving 'N.latitude' and 'W.longitude' for each place.
#'   \item region: Which of 4 climate zones contain each place: Atlantic, Pacific, Continental, Arctic.
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
#' # create model with 4 subregressions ans 2 coefficients per regression
#' model <- list(tempav = list(type = "Func_CS", paramStr = "nSub: 4, nCoeff: 2"),
#'               precav = list(type = "Func_CS", paramStr = "nSub: 4, nCoeff: 2"))
#' 
#' # create algo
#' algo <- createAlgo()
#' 
#' # run clustering
#' resLearn <- mixtCompLearn(dat, model, algo, nClass = 2:4, criterion = "ICL", nRun = 3, nCore = 1)
#' 
#' summary(resLearn)
#' 
#' plot(resLearn) 
#' 
#' getPartition(resLearn)
#' getTik(resLearn, log = FALSE)
#' 
#' }
#'   
NULL


#' Prostate Cancer Data
#'
#' @name prostate
#' @docType data
#' @keywords data
#' @family data
#' 
#' @description This data set was obtained from a randomized clinical trial comparing four treatments for n = 506 patients 
#' with prostatic cancer grouped on clinical criteria into two Stages 3 and 4 of the disease.
#' 
#' @usage data(prostate)
#' 
#' @format A list containing of 2 elements \emph{data} and \emph{model}.
#' \emph{data} contains 506 individuals described by 12 variables: 
#' \itemize{
#'   \item Age: Age (Continuous) 
#'   \item HG: Index of tumour stage and histolic grade (Continuous) 
#'   \item Wt: Weight (Continuous) 
#'   \item AP: Serum prostatic acid phosphatase C (Continuous) 
#'   \item SBP: Systolic blood pressure (Continuous) 
#'   \item PF: Performance rating (Categorical) 
#'   \item DBP: Diastolic blood pressure (Continuous) 
#'   \item HX: Cardiovascular disease history (Categorical)
#'   \item SG: Serum haemoglobin (Continuous) 
#'   \item BM: Bone metastasis (Categorical)
#'   \item SZ: Size of primary tumour (Continuous)
#'   \item EKG: Electrocardiogram code (Categorical)
#' }
#' 
#' @source 
#' Yakovlev, Goot and Osipova (1994), The choice of cancer treatment based on covariate information. Statist. Med., 13: 1575-1581. doi:10.1002/sim.4780131508
#' 
#' 
#' @examples
#' \donttest{
#' data(prostate)
#' 
#' algo <- createAlgo(nInitPerClass = 50)
#' 
#' # run clustering
#' resLearn <- mixtCompLearn(prostate$data, prostate$model, algo, nClass = 2:5, criterion = "ICL",
#'                           nRun = 3, nCore = 1)
#' 
#' summary(resLearn)
#' 
#' plot(resLearn) 
#' 
#' }
#'   
NULL