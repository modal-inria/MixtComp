context("Subdir creation")

test_that("Expand handling", {
  expect_error(create_subdirectories("",nClass = 1))
  dir.create(paste0(getwd(),"/wdir"),showWarnings = F)
  expect_error(create_subdirectories("tests/wdir",nClass = 0))
  expect_error(create_subdirectories("tests/wdir/",nClass = 0))

  create_subdirectories("wdir/",nClass = 4)
  expect_equal(length(list.dirs("wdir/",recursive = F)),4)
  unlink("wdir/",recursive=T)
})


