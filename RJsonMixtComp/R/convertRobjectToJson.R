

#' @title Convert an R object to json
#'
#' @description convert an R object to a json file to use with \link{JsonMixtCompPredict} function
#'
#' @param outMixtComp output object of \link{JsonMixtCompCluster} or \link{JsonMixtCompPredict}  functions.
#' @param filename name of the variable to get parameter
#'
#' @export
writeJsonParamFile <- function(outMixtComp, filename)
{
  
  # TODO ne marche qu'avec du gaussien
  res2 <- outMixtComp
  res2$mixture = NULL
  res2$strategy = NULL
  res2$variable$param$z_class$pi$log = NULL
  res2$variable$data = NULL
  
  varName <- setdiff(names(outMixtComp$variable$type), "z_class")
  
  res2$variable$param$z_class$pi$stat <- list(median = outMixtComp$variable$param$z_class$pi$stat[,1],
                                              "q 2.5%" = outMixtComp$variable$param$z_class$pi$stat[,2],
                                              "q 97.5%" = outMixtComp$variable$param$z_class$pi$stat[,3])
  
  
  for(nom in varName)
  {
    res2$variable$param[[nom]]$NumericalParam$stat <- list(median = outMixtComp$variable$param[[nom]]$NumericalParam$stat[,1],
                                                           "q 2.5%" = outMixtComp$variable$param[[nom]]$NumericalParam$stat[,2],
                                                           "q 97.5%" = outMixtComp$variable$param[[nom]]$NumericalParam$stat[,3])
    
  }
  
  
  write(x = jsonlite::toJSON(res2, auto_unbox = TRUE), filename)
  
}
