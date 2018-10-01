context("Mixtcomp Hierarchical launch - Learn mode")

test_that("Mixtcomp Hierarchical launch - Learn mode", {

  dir.create(paste0(getwd(),"/wdir"),showWarnings = F)
  set.seed(seed=1)
  generate_data(size=1000,ratio_test_training = 0.5, path_dir_output = "wdir",correlation = FALSE)

  expect_true(file.exists(paste0(getwd(),"/wdir/generated_data_training.csv")))
  expect_true(nrow(read.csv(paste0(getwd(),"/wdir/generated_data_training.csv")))==500)

  # Launch Mixtcomp Learn

  launch_Mixtcomp_Hierarchical(data_path = "wdir/generated_data_training.csv",
                                               descriptor_path = "wdir/descriptor.csv",
                                               nClass = 2,
                                               depth = 1,
                                               mcStrategy = list(
                                                 nbBurnInIter = 2,
                                                 nbIter = 2,
                                                 nbGibbsBurnInIter = 2,
                                                 nbGibbsIter = 2
                                               ),
                                               nCore=8)

  clusters = aggregate_clusters("wdir/generated_data_training/")
  expect_equal(length(table(clusters)),4)

})


context("Mixtcomp Hierarchical launch - Predict mode")

test_that("Mixtcomp Hierarchical launch - Predict mode", {

  expect_true(file.exists(paste0(getwd(),"/wdir/generated_data_test.csv")))
  expect_true(nrow(read.csv(paste0(getwd(),"/wdir/generated_data_test.csv")))==500)

  # Launch Mixtcomp Learn

  launch_Mixtcomp_Hierarchical_predict(data_path = "wdir/generated_data_test.csv",
                                       param_dir = "wdir/generated_data_training",
                                       mcStrategy = list(
                                         nbBurnInIter = 2,
                                         nbIter = 2,
                                         nbGibbsBurnInIter = 2,
                                         nbGibbsIter = 2
                                       ))

  clusters = aggregate_clusters("wdir/generated_data_test/")
  expect_equal(length(table(clusters)),4)
  unlink("wdir/",recursive=T)
})

context("Hierachical Strategy Effects")

test_that("Mixtcomp Strategy Effects", {
  dir.create(paste0(getwd(),"/wdir"),showWarnings = F)
  set.seed(seed=1)
  generate_data(size=100,ratio_test_training = 0, path_dir_output = "wdir")

  expect_true(file.exists(paste0(getwd(),"/wdir/generated_data_training.csv")))
  expect_true(nrow(read.csv(paste0(getwd(),"/wdir/generated_data_training.csv")))==100)

  # Launch Mixtcomp Learn

  launch_Mixtcomp_Hierarchical(data_path = "wdir/generated_data_training.csv",
                               descriptor_path = "wdir/descriptor.csv",
                               nClass = 2,
                               depth = 1,
                               mcStrategy = list(
                                 nbBurnInIter = 2,
                                 nbIter = 2,
                                 nbGibbsBurnInIter = 2,
                                 nbGibbsIter = 2),
                               strategy = list(var = NULL,
                                               threshold_nInd = 100,
                                               threshold_purity = 2),
                               nCore=2)

  clusters = aggregate_clusters("wdir/generated_data_training/")
  expect_equal(length(table(clusters)),2)
  unlink("wdir/",recursive=T)

  dir.create(paste0(getwd(),"/wdir"),showWarnings = F)
  set.seed(seed=1)
  generate_data(size=100,ratio_test_training = 0, path_dir_output = "wdir")
  launch_Mixtcomp_Hierarchical(data_path = "wdir/generated_data_training.csv",
                               descriptor_path = "wdir/descriptor.csv",
                               nClass = 2,
                               depth = 1,
                               mcStrategy = list(
                                 nbBurnInIter = 2,
                                 nbIter = 2,
                                 nbGibbsBurnInIter = 2,
                                 nbGibbsIter = 2),
                               strategy = list(var = "C1",
                                               threshold_nInd = 1,
                                               threshold_purity = 0.1601),
                               nCore=1)
  clusters = aggregate_clusters("wdir/generated_data_training/")

  expect_equal(length(table(clusters)),2)
  unlink("wdir/",recursive=T)

})
