context("Subdir creation")

test_that("Expand handling", {
  expect_error(create_subdirectories("",nClass = 1))
  dir.create(paste0(getwd(),"/wdir"),showWarnings = F)
  expect_error(create_subdirectories("tests/wdir",nClass = 0))
  expect_error(create_subdirectories("tests/wdir/",nClass = 0))

  random_2_5 <- sample(2:5,s=1)
  create_subdirectories("wdir/",nClass = random_2_5)
  expect_equal(length(list.dirs("wdir/",recursive = F)),random_2_5)
  unlink("wdir/",recursive=T)
})
