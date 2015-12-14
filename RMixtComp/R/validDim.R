#' Generate a random matrix of a given size and write it in the test directory. Used to
#' test validDim
generateDummyData <- function(fileName,
                              nRow,
                              nCol) {
  mat <- matrix(rexp(nRow * nCol,
                     rate = .1),
                ncol = nCol)
  
  write.table(x = mat,
              file = fileName,
              sep = ";",
              row.names = FALSE,
              col.names = FALSE)
}

#' Check if the data in fileName is smaller than the provided dimensions
validDim <- function(fileName,
                     maxCol = NA_integer_,
                     maxRow = NA_integer_) {
  warnLog <- ""
  
  mat <- read.table(fileName,
                    header = FALSE,
                    sep = ";")
  
  nRow <- nrow(mat) 
  nCol <- ncol(mat)
    
  if (!is.na(maxRow) && maxRow < nRow) {
    warnLog <- paste0(warnLog,
                     "The data file has ", nRow, " rows, but the maximum number of rows authorized is: ", maxRow, ".\n")
  }
  if (!is.na(maxCol) && maxCol < nCol) {
    warnLog <- paste0(warnLog,
                      "The data file has ", nCol, " columns, but the maximum number of columns authorized is: ", maxCol, ".\n")
  }

  return(warnLog)
}