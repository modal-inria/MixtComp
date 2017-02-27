
# Check if the data in fileName is smaller than the provided dimensions
validDim <- function(fileName,
                     maxRow = NA_integer_,
                     maxCol = NA_integer_) {
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