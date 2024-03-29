% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/MIXTCOMP_methods.R
\name{summary.MixtCompLearn}
\alias{summary.MixtCompLearn}
\title{MixtCompLearn Object Summaries}
\usage{
\method{summary}{MixtCompLearn}(object, nClass = NULL, ...)
}
\arguments{
\item{object}{\emph{MixtCompLearn} object}

\item{nClass}{number of classes of the model to print}

\item{...}{Not used.}
}
\value{
No return value, called for side effects
}
\description{
Summary of a \emph{MixtCompLearn} object
}
\examples{
data(iris)

# run RMixtComp in unsupervised clustering mode and in basic mode
resLearn <- mixtCompLearn(iris[, -5], nClass = 2:4, nCore = 1)

summary(resLearn)
summary(resLearn, nClass = 3)

}
\seealso{
\code{\link{mixtCompLearn}} \code{print.MixtCompLearn}
}
\author{
Quentin Grimonprez
}
