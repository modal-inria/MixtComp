context("Data generator")

test_that("poisson missing data",{
  
  out <- poissonHideData(FALSE, 0.5)
  expect_equal(out, "?")

  
  out <- poissonHideData(TRUE, 0.5)
  expect_equal(out, 0.5)
  
})



test_that("gaussian missing data",{
  
  for(i in 1:10)
  {
    param <- list(mean = 0, sd = 1)
    out <- gaussianHideData(FALSE, 0.5, param)
    
    
    if(out == "?")
    {
      expect_equal(out, "?") # dummy test
    }else{
      expect_equal(substr(out, 1, 1), "[")
      expect_equal(substr(out, nchar(out), nchar(out)), "]")
      expect_equal(grep("[:]", out), 1)
      
      nombre <- strsplit(gsub("[[]|[]]", "", out), ":")[[1]]
      expect_silent(as.numeric(nombre))# if they are numeric there is no warnings
    } 
  }
  
  out <- gaussianHideData(TRUE, 0.5, param)
  expect_equal(out, 0.5)
  
})




test_that("categorical missing data",{
  
  for(i in 1:10)
  {
    param <- c(0.2, 0.3, 0.5)
    out <- categoricalHideData(FALSE, 2, param)
    
    if(out == "?")
    {
      expect_equal(out, "?") # dummy test
    }else{
      expect_equal(substr(out, 1, 1), "{")
      expect_equal(substr(out, nchar(out), nchar(out)), "}")
      expect_equal(grep(",", out), 1)
      
      nombre <- strsplit(gsub("[{]|[}]", "", out), ",")[[1]]
      expect_equal(length(nombre), 2)
      expect_silent(as.numeric(nombre))# if they are numeric there is no warnings
    }
  }
  
  
  param <- c(0.2, 0.3, 0.5)
  out <- categoricalHideData(TRUE, 2, param)
  expect_equal(out, 2)
  
  
  param <- c(0.5, 0.5)
  out <- categoricalHideData(FALSE, 2, param)
  expect_equal(out, "?")
})



test_that("test fonctionnel : dataGeneratorNew", {
  
  var <- list()
  var$z_class <- zParam()
  var$Rank1 <- rankParam("Rank1")
  var$Ordinal1 <- ordinalParam("Ordinal1")
  var$Functional1 <- functionalParam1sub("Functional1")
  var$Poisson1 <- poissonParam("Poisson1")
  var$Gaussienne1 <- gaussianParam("Gaussian1")
  var$Categorical1 <- categoricalParam("Categorical1")
  
  dat <- dataGeneratorNew(100, 0.8, var) 
  
  expect_length(dat, 3)
  expect_equal(colnames(dat$data), c("z_class", "Rank1", "Ordinal1", "Functional1", "Poisson1", "Gaussian1", "Categorical1"))
  expect_equal(colnames(dat$descriptor), c("z_class", "Rank1", "Ordinal1", "Functional1", "Poisson1", "Gaussian1", "Categorical1"))
  expect_equal(as.character(dat$descriptor[1, 1]), "LatentClass")
  expect_equal(as.character(dat$descriptor[1, 2]), "Rank")
  expect_equal(as.character(dat$descriptor[1, 3]), "Ordinal")
  expect_equal(as.character(dat$descriptor[1, 4]), "Functional")
  expect_equal(as.character(dat$descriptor[1, 5]), "Poisson_k")
  expect_equal(as.character(dat$descriptor[1, 6]), "Gaussian_sjk")
  expect_equal(as.character(dat$descriptor[1, 7]), "Categorical_pjk")
  for(i in (1:7)[-4])
    expect_equal(as.character(dat$descriptor[2, i]), "")
  expect_equal(as.character(dat$descriptor[2,4]), "nSub: 1, nCoeff: 2")
  expect_equal(dim(dat$data), c(100, 7))
})

