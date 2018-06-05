# convertis une fonctionnelle du format MixtComp à une amtrice de deux colonnes : temps et valeurs
convertFunctional <- function(x)
{
  return(do.call(rbind, lapply(strsplit(strsplit(as.character(x), ",")[[1]], ":"), as.numeric)))
}

# créé le format fonctionnel de mixtcomp
createFunctional <- function(temps, valeur)
{
  return(paste(paste(temps, valeur, sep = ":"), collapse = ","))
}

# plot d'une fonctionnelle au format MixtComp
plotFunctional <- function(x, ...)
{
  newX <- convertFunctional(x)

  plot(newX[,1], newX[,2], type = "s", ...)
}

linesFunctional <- function(x, ...)
{
  newX <- convertFunctional(x)

  lines(newX[,1], newX[,2], type = "s", ...)
}

# data_path <- "../../clustering_on_fonctional/outputs/data_mixtcomp.csv"
# data <- read.csv(data_path,sep=";")
# convertFunctional(data[4,1])
