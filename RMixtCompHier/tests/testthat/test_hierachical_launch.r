# MixtComp version 4.0  - july 2019
# Copyright (C) Inria - Université de Lille - CNRS

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
# 
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>
 

# @author Étienne Goffinet
context("Mixtcomp Hierarchical launch - oldMC")

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


test_that("Mixtcomp Hierarchical launch - Predict mode", {

  expect_true(file.exists(paste0(getwd(),"/wdir/generated_data_test.csv")))
  expect_true(nrow(read.csv(paste0(getwd(),"/wdir/generated_data_test.csv"))) == 500)

  launch_Mixtcomp_Hierarchical_predict(data_path = "wdir/generated_data_test.csv",
                                       param_dir = "wdir/generated_data_training",
                                       mcStrategy = list(
                                         nbBurnInIter = 2,
                                         nbIter = 2,
                                         nbGibbsBurnInIter = 2,
                                         nbGibbsIter = 2
                                       ))

  clusters = aggregate_clusters("wdir/generated_data_test/")
  expect_equal(length(table(clusters)), 4)
  unlink("wdir/", recursive = TRUE)
})



context("Mixtcomp Hierarchical launch - new MC")


test_that("Mixtcomp Hierarchical launch - Learn mode - new MC", {

  dir.create(paste0(getwd(),"/wdir"), showWarnings = FALSE)
  set.seed(1)
  generate_data(size = 1000, ratio_test_training = 0.5, path_dir_output = "wdir", correlation = FALSE, oldMC = FALSE)

  dat <- read.table(paste0(getwd(),"/wdir/generated_data_training.csv"), sep = ";", header = TRUE)
  expect_true(file.exists(paste0(getwd(),"/wdir/generated_data_training.csv")))
  expect_true(nrow(dat) == 500)

  # Launch Mixtcomp Learn
  launch_Mixtcomp_Hierarchical(data_path = "wdir/generated_data_training.csv",
                               descriptor_path = "wdir/descriptor.csv",
                               nClass = 2, depth = 1,
                               mcStrategy = list(nbBurnInIter = 2,
                                                 nbIter = 2,
                                                 nbGibbsBurnInIter = 2,
                                                 nbGibbsIter = 2),
                               strategy = list(var = "C1",
                                               threshold_nInd = 1,
                                               threshold_purity = 0.9),
                               nCore = 2, oldMC = FALSE)

  clusters = aggregate_clusters("wdir/generated_data_training/")
  expect_equal(length(table(clusters)), 4)
  expect_equal(dim(aggregate_classification_probabilities("wdir/generated_data_training/", depth = 2)), c(500, 4))
  expect_equal(aggregate_completed("wdir/generated_data_training/", var = "C1", depth = NULL), dat$C1)
  expect_equal(aggregate_completed("wdir/generated_data_training/", var = "C1", depth = 1), dat$C1)

})


test_that("Mixtcomp Hierarchical launch - Predict mode - new MC", {

  expect_true(file.exists(paste0(getwd(),"/wdir/generated_data_test.csv")))
  expect_true(nrow(read.csv(paste0(getwd(),"/wdir/generated_data_test.csv"))) == 500)


  launch_Mixtcomp_Hierarchical_predict(data_path = "wdir/generated_data_test.csv",
                                       param_dir = "wdir/generated_data_training",
                                       mcStrategy = list(nbBurnInIter = 2,
                                                         nbIter = 2,
                                                         nbGibbsBurnInIter = 2,
                                                         nbGibbsIter = 2))

  clusters = aggregate_clusters("wdir/generated_data_test/")
  expect_equal(length(table(clusters)), 4)
  expect_equal(dim(aggregate_classification_probabilities("wdir/generated_data_test/", depth = 2)), c(500, 4))
  expect_warning(aggregate_completed("wdir/generated_data_test/", var = "C1", depth = NULL), regexp = NA)
  expect_warning(aggregate_completed("wdir/generated_data_test/", var = "C1", depth = 1), regexp = NA)
  unlink("wdir/", recursive = TRUE)
})




context("Hierachical Strategy Effects")

test_that("Mixtcomp Strategy Effects", {
  skip("marche pas")
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

