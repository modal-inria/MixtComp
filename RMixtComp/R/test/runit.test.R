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