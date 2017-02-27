context("Data generator")

test_that("test fonctionnel : dataGeneratorNew", {
  
  var <- list()
  var$z_class <- zParam()
  var$Rank1 <- rankParam("Rank1")
  var$Ordinal1 <- ordinalParam("Ordinal1")
  var$Functional1 <- functionalParam1sub("Functional1")

  dat <- dataGeneratorNew(100, 0.8, var) 
  
  expect_length(dat, 3)
  expect_equal(dat$data[1,], c("z_class", "Rank1", "Ordinal1", "Functional1"))
  expect_equal(dat$descriptor[1,], c("z_class", "Rank1", "Ordinal1", "Functional1"))
  expect_equal(dat$descriptor[2,], c("LatentClass", "Rank", "Ordinal", "Functional"))
  expect_equal(dat$descriptor[3,], c("", "", "", "nSub: 1, nCoeff: 2"))
  expect_equal(dim(dat$data), c(101, 4))
})

