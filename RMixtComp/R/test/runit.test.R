test.validDim <- function() {
  testFile <- "test/mat.csv"
  
  generateDummyData(testFile,
                    10,
                    10)
  
  checkEquals(nchar(validDim(testFile)) == 0,
              TRUE)
  checkEquals(nchar(validDim(testFile,
                             12,
                             12)) == 0,
              TRUE)
  checkEquals(nchar(validDim(testFile,
                             5,
                             12)) > 0,
              TRUE)
  checkEquals(nchar(validDim(testFile,
                             12,
                             5)) > 0,
              TRUE)
  checkEquals(nchar(validDim(testFile,
                             5,
                             5)) > 0,
              TRUE)
}

test.allSeg <- function() {
  nbPos <- 5
  allTrue <- TRUE
  
  for (i in 1:nbPos) {
    for (j in i:nbPos) {
      for (y in i:j) {
        for (z in 1:2) {
          for (m in 1:nbPos) {
            param <- list(mu = m)
            
            currSeg <- allSeg(y,
                              c(i, j),
                              param,
                              nbPos)
            
            e <- eSample(z,
                         currSeg)
            
            seg <- c(currSeg$firstIndSeg[e],
                     currSeg$firstIndSeg[e] + currSeg$lengthSeg[e] - 1)
            
            allTrue <- allTrue && (seg[1] <= seg[2])
          }
        }
      }
    }
  }
  
  checkEquals(allTrue, TRUE)
}