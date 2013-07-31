#'
#'@include Strategy.R
#' 
#' 
NULL
#' Common Input/Output options.
#' 
#' This class contains all the input options and common output options for all kinds of data-sets (Binary, 
#' Contingency and Continuous).
#' 
#' \describe{
#' \item{The following are the various input options:}{}
#' \item{data: }{Input data.}
#' \item{datatype: }{Type of data.}
#' \item{semisupervised: }{Boolean value specifying if Co-clustering is semi-supervised or not.}
#' \item{model: }{Model to be run for co-clustering.}
#' \item{nbcocluster: }{Number of row and column clusters.}
#' \item{strategy: }{Input strategy.}
#' \item{The following are the various common output options:}{}
#' \item{message: }{Status returned.}
#' \item{rowproportions: }{Vector of row proportions.}
#' \item{colproportions: }{Vector of column proportions.}
#' \item{rowclass: }{Vector of assigned row cluster to each row.}
#' \item{colclass: }{Vector of assigned column cluster to each column.}
#' \item{likelihood: }{Final log-likelihood.}
#' \item{rowposteriorprob: }{Final posterior probabilities for rows.}
#' \item{colposteriorprob: }{Final posterior probabilities for columns.}
#' 
#' }
#' 
#' @name CommonOptions-class
#' @exportClass CommonOptions
#' 

setClass(
		Class = "CommonOptions",
		representation = representation(
				data = "matrix",
        rowlabels = "numeric",
        collabels = "numeric",
				datatype = "character",
				model = "character",
				nbcocluster = "numeric",
        semisupervised = "logical",
				strategy = "strategy",
				message = "character",
				successful = "logical",
				rowproportions = "numeric",
				columnproportions = "numeric",
				rowclass = "numeric",
				colclass = "numeric",
				rowposteriorprob = "numeric",
				colposteriorprob = "numeric",
				likelihood = "numeric",
				coclusterdata = "matrix"
		),
		prototype = prototype(
				data = matrix(nrow=0,ncol=0),
        rowlabels = numeric(0),
        collabels = numeric(0),
				datatype = character(0),
				model = character(0),
				nbcocluster = numeric(0),
        semisupervised = logical(0),
				message = character(0),
				successful = logical(0),
				rowproportions = numeric(0),
				columnproportions = numeric(0),
				rowclass = numeric(0),
				colclass = numeric(0),
				rowposteriorprob = numeric(0),
				colposteriorprob = numeric(0),
				likelihood = numeric(0),
				coclusterdata = matrix(nrow=0,ncol=0)
		)
)

#'
#' Binary input/output options
#' 
#' This class contains all the input options as well as the estimated paramters for Binary data-set. It inherits
#' from base class \code{\linkS4class{CommonOptions}}. The class contains following output parameters given in 'Details' along
#' with the parameters in base class. 
#' 
#' \describe{
#' \item{classmean:}{The mean value of each co-cluster.}
#' \item{classdispersion:}{The dispersion of each co-cluster.}
#' }
#' 
#' @name BinaryOptions-class
#' @exportClass BinaryOptions
#' 
#' 
setClass(
		Class = "BinaryOptions",
		contains = "CommonOptions",
		representation = representation(
				classmean = "matrix",
				classdispersion = "matrix"
		),
		
		prototype = prototype(
				classmean = matrix(nrow=0,ncol=0),
				classdispersion = matrix(nrow=0,ncol=0)
		)
)

#'
#' Contingency input/output options
#' 
#' This class contains all the input options as well as the estimated paramters for Contingency data-set.It inherits
#' from base class \code{\linkS4class{CommonOptions}}. The class contains following output parameters given in 'Details' along
#' with the parameters in base class.
#' 
#' \describe{
#' \item{classgamma:}{The value of poisson parameter (gamma) for each co-cluster.}  
#' \item{datamui:}{Rows effect (if known).}
#' \item{datanuj:}{Columns effect (if known).}
#' }
#' 
#' @name ContingencyOptions-class
#' @exportClass ContingencyOptions
#' 
setClass(
		Class = "ContingencyOptions",
		contains = "CommonOptions",
		representation = representation(
				classgamma = "matrix",
				datamui = "numeric",
				datanuj = "numeric"
		),
		prototype = prototype(
				classgamma = matrix(nrow=0,ncol=0),
				datamui = numeric(0),
				datanuj = numeric(0)
		)		
)

#' Continuous input/output options
#' 
#' This class contains all the input options as well as the estimated paramters for Continuous data-sets. It inherits
#' from base class \code{\linkS4class{CommonOptions}}. The class contains following output parameters given in 'Details' along
#' with the parameters in base class.
#' 
#' \describe{
#' \item{classmean:}{The mean value of each co-cluster.}  
#' \item{classvariance:}{The variance of each co-cluster.} 
#' }
#' 
#' @name ContinuousOptions-class
#' @exportClass ContinuousOptions
#' 
setClass(
		Class = "ContinuousOptions",
		contains = "CommonOptions",
		representation = representation(
				classmean = "matrix",
				classvariance = "matrix"
		),
		prototype = prototype(
				classmean = matrix(nrow=0,ncol=0),
				classvariance = matrix(nrow=0,ncol=0)
		)
)

#'
#' Categorical input/output options
#' 
#' This class contains all the input options as well as the estimated paramters for categorical data-set. It inherits
#' from base class \code{\linkS4class{CommonOptions}}. The class contains following output parameters given in 'Details' along
#' with the parameters in base class. 
#' 
#' \describe{
#' \item{classmean:}{The categorical distribution of each co-cluster}
#' }
#' 
#' @name BinaryOptions-class
#' @exportClass BinaryOptions
#' 
#' 
setClass(
  Class = "CategoricalOptions",
  contains = "CommonOptions",
  representation = representation(
    classmean = "list"
  ),
  
  prototype = prototype(
    classmean = list(0)
  )
)


#' Getter method for Rcoclust output
#' 
#' This is overloading of square braces to extract values of various slots of the output from \code{\link{cocluster}}.
#' 
#' @param x object from which to extract element(s) or in which to replace element(s).
#' @param i the name of the element we want to extract or replace.
#' @param j if the element designing by i is complex, j specifying elements to extract or replace.
#' 
#' @name [
#' @rdname getter-methods
#' 
NULL


#' @rdname getter-methods
#' @aliases [,BinaryOptions-method
#' 

setMethod(
		f = "[",
		signature = "BinaryOptions",
		definition = function(x,i,j,drop) {
			switch(EXPR=i,
					"data"={return (x@data)},
          "coclusterdata"={return (x@coclusterdata)},
					"datatype"={return (x@datatype)},
          "semisupervised"={return (x@semisupervised)},
					"model"={return (x@model)}, 
					"strategy"={return (x@strategy)},
					"message"={return (x@message)}, 
					"rowproportions"={return (x@rowproportions)},
					"colproportions"={return (x@colproportions)},
					"rowclass"={return (x@rowclass)},
					"colclass"={return (x@colclass)},
					"likelihood"={return (x@likelihood)},
					"rowposteriorprob"={return (x@rowposteriorprob)},
					"colposteriorprob"={return (x@colposteriorprob)},
					"classmean"={return (x@classmean)},
					"classdispersion"={return (x@classdispersion)},
					stop("Invalid slot name.")
			)
		}
)

#' @rdname getter-methods
#' @aliases [,ContingencyOptions-method
#' 


setMethod(
		f = "[",
		signature = "ContingencyOptions",
		definition = function(x,i,j,drop) {
			switch(EXPR=i,
					"data"={return (x@data)},
			    "coclusterdata"={return (x@coclusterdata)},
          "datatype"={return (x@datatype)},
			    "semisupervised"={return (x@semisupervised)},
					"model"={return (x@model)}, 
					"strategy"={return (x@strategy)},
					"message"={return (x@message)}, 
					"rowproportions"={return (x@rowproportions)},
					"colproportions"={return (x@colproportions)},
					"rowclass"={return (x@rowclass)},
					"colclass"={return (x@colclass)},
					"likelihood"={return (x@likelihood)},
					"rowposteriorprob"={return (x@rowposteriorprob)},
					"colposteriorprob"={return (x@colposteriorprob)},
					"classgamma"={return (x@classgamma)},
					stop("Invalid slot name.")
			)
			
		}
)

#' @rdname getter-methods
#' @aliases [,ContinuousOptions-method
#' 


setMethod(
		f = "[",
		signature = "ContinuousOptions",
		definition = function(x,i,j,drop) {
			switch(EXPR=i,
					"data"={return (x@data)},
			    "coclusterdata"={return (x@coclusterdata)},
					"datatype"={return (x@datatype)},
			    "semisupervised"={return (x@semisupervised)},
					"model"={return (x@model)}, 
					"strategy"={return (x@strategy)},
					"message"={return (x@message)}, 
					"rowproportions"={return (x@rowproportions)},
					"colproportions"={return (x@colproportions)},
					"rowclass"={return (x@rowclass)},
					"colclass"={return (x@colclass)},
					"likelihood"={return (x@likelihood)},
					"rowposteriorprob"={return (x@rowposteriorprob)},
					"colposteriorprob"={return (x@colposteriorprob)},
					"classmean"={return (x@classmean)},
					"classvariance"={return (x@classvariance)},
					stop("Invalid slot name.")
			)
			
		}
)


#' @rdname getter-methods
#' @aliases [,CategoricalOptions-method
#' 

setMethod(
  f = "[",
  signature = "CategoricalOptions",
  definition = function(x,i,j,drop) {
    switch(EXPR=i,
           "data"={return (x@data)},
           "coclusterdata"={return (x@coclusterdata)},
           "datatype"={return (x@datatype)},
           "semisupervised"={return (x@semisupervised)},
           "model"={return (x@model)}, 
           "strategy"={return (x@strategy)},
           "message"={return (x@message)}, 
           "rowproportions"={return (x@rowproportions)},
           "colproportions"={return (x@colproportions)},
           "rowclass"={return (x@rowclass)},
           "colclass"={return (x@colclass)},
           "likelihood"={return (x@likelihood)},
           "rowposteriorprob"={return (x@rowposteriorprob)},
           "colposteriorprob"={return (x@colposteriorprob)},
           "classmean"={return (x@classmean)},
           stop("Invalid slot name.")
    )
  }
)