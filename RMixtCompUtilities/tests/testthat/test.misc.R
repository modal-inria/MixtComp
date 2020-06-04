# MixtComp version 4 - july 2019
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


# @author Quentin Grimonprez
context("Misc functions")


test_that("reduceRMixtCompOutput works in FALSE FALSE", {
  res = list(variable = list(type = list(z_class = "LatentClass",
                                         var1 = "Gaussian",
                                         var2 = "Multinomial",
                                         var3 = "Poisson",
                                         var4 = "NegativeBinomial",
                                         var5 = "Weibull",
                                         var6 = "Rank_ISR",
                                         var7 = "Func_SharedAlpha_CS",
                                         var8 = "Func_CS"),
                             param = list(z_class = list(stat = 1, log = 1),
                                          var1 = list(stat = 1, log = 1),
                                          var2 = list(stat = 1, log = 1),
                                          var3 = list(stat = 1, log = 1),
                                          var4 = list(stat = 1, log = 1),
                                          var5 = list(stat = 1, log = 1),
                                          var6 = list(mu = list(stat = 1, log = 1),
                                                      pi = list(stat = 1, log = 1)),
                                          var7 = list(alpha = list(stat = 1, log = 1),
                                                      beta = list(stat = 1, log = 1),
                                                      sd = list(stat = 1, log = 1)),
                                          var8 = list(alpha = list(stat = 1, log = 1),
                                                      beta = list(stat = 1, log = 1),
                                                      sd = list(stat = 1, log = 1))),
                             data = list(z_class = list(completed = 1:10),
                                         var1 = list(completed = 1:10, a = 1),
                                         var2 = list(time = 1:10, data = 1, a = 1))))
  
  class(res) = "MixtComp"
  
  
  resOut <- reduceRMixtCompOutput(res, completed = FALSE, log = FALSE)
  
  expect_equal(resOut, res)
  
  
  res2 <- list(variable = res$variable, nClass = 2:3, res = list(NULL, res, res))
  class(res2) = "MixtCompLearn"
  
  resOut <- reduceRMixtCompOutput(res2, completed = FALSE, log = FALSE)
  
  expect_equal(resOut, res2)
  
})



test_that("reduceRMixtCompOutput works in TRUE TRUE", {
  res = list(variable = list(type = list(z_class = "LatentClass",
                                         var1 = "Gaussian",
                                         var2 = "Multinomial",
                                         var3 = "Poisson",
                                         var4 = "NegativeBinomial",
                                         var5 = "Weibull",
                                         var6 = "Rank_ISR",
                                         var7 = "Func_SharedAlpha_CS",
                                         var8 = "Func_CS"),
                             param = list(z_class = list(stat = 1, log = 1),
                                          var1 = list(stat = 1, log = 1),
                                          var2 = list(stat = 1, log = 1),
                                          var3 = list(stat = 1, log = 1),
                                          var4 = list(stat = 1, log = 1),
                                          var5 = list(stat = 1, log = 1),
                                          var6 = list(mu = list(stat = 1, log = 1),
                                                      pi = list(stat = 1, log = 1)),
                                          var7 = list(alpha = list(stat = 1, log = 1),
                                                      beta = list(stat = 1, log = 1),
                                                      sd = list(stat = 1, log = 1)),
                                          var8 = list(alpha = list(stat = 1, log = 1),
                                                      beta = list(stat = 1, log = 1),
                                                      sd = list(stat = 1, log = 1))),
                             data = list(z_class = list(completed = 1:10),
                                         var1 = list(completed = 1:10, a = 1),
                                         var2 = list(time = 1:10, data = 1, a = 1))))
  
  class(res) = "MixtComp"
  
  expectedRes = list(variable = list(type = list(z_class = "LatentClass",
                                                 var1 = "Gaussian",
                                                 var2 = "Multinomial",
                                                 var3 = "Poisson",
                                                 var4 = "NegativeBinomial",
                                                 var5 = "Weibull",
                                                 var6 = "Rank_ISR",
                                                 var7 = "Func_SharedAlpha_CS",
                                                 var8 = "Func_CS"),
                                     param = list(z_class = list(stat = 1),
                                                  var1 = list(stat = 1),
                                                  var2 = list(stat = 1),
                                                  var3 = list(stat = 1),
                                                  var4 = list(stat = 1),
                                                  var5 = list(stat = 1),
                                                  var6 = list(mu = list(stat = 1),
                                                              pi = list(stat = 1)),
                                                  var7 = list(alpha = list(stat = 1),
                                                              beta = list(stat = 1),
                                                              sd = list(stat = 1)),
                                                  var8 = list(alpha = list(stat = 1),
                                                              beta = list(stat = 1),
                                                              sd = list(stat = 1))),
                                     data = list(z_class = list(completed = 1:10),
                                                 var1 = list(a = 1),
                                                 var2 = list(a = 1))))
  
  class(expectedRes) = "MixtComp"
  
  resOut <- reduceRMixtCompOutput(res, completed = TRUE, log = TRUE)
  
  expect_equal(resOut, expectedRes)
  
  
  res2 <- list(variable = res$variable, nClass = 2:3, res = list(NULL, res, res))
  class(res2) = "MixtCompLearn"
  
  expectedRes2 <- list(variable = expectedRes$variable, nClass = 2:3, res = list(NULL, expectedRes, expectedRes))
  resOut <- reduceRMixtCompOutput(res2, completed = TRUE, log = TRUE)
  
  expect_equivalent(resOut, expectedRes2)
  
})

test_that("reduceRMixtCompOutput works in TRUE FALSE", {
  res = list(variable = list(type = list(z_class = "LatentClass",
                                         var1 = "Gaussian",
                                         var2 = "Multinomial",
                                         var3 = "Poisson",
                                         var4 = "NegativeBinomial",
                                         var5 = "Weibull",
                                         var6 = "Rank_ISR",
                                         var7 = "Func_SharedAlpha_CS",
                                         var8 = "Func_CS"),
                             param = list(z_class = list(stat = 1, log = 1),
                                          var1 = list(stat = 1, log = 1),
                                          var2 = list(stat = 1, log = 1),
                                          var3 = list(stat = 1, log = 1),
                                          var4 = list(stat = 1, log = 1),
                                          var5 = list(stat = 1, log = 1),
                                          var6 = list(mu = list(stat = 1, log = 1),
                                                      pi = list(stat = 1, log = 1)),
                                          var7 = list(alpha = list(stat = 1, log = 1),
                                                      beta = list(stat = 1, log = 1),
                                                      sd = list(stat = 1, log = 1)),
                                          var8 = list(alpha = list(stat = 1, log = 1),
                                                      beta = list(stat = 1, log = 1),
                                                      sd = list(stat = 1, log = 1))),
                             data = list(z_class = list(completed = 1:10),
                                         var1 = list(completed = 1:10, a = 1),
                                         var2 = list(time = 1:10, data = 1, a = 1))))
  
  class(res) = "MixtComp"
  
  expectedRes = list(variable = list(type = list(z_class = "LatentClass",
                                                 var1 = "Gaussian",
                                                 var2 = "Multinomial",
                                                 var3 = "Poisson",
                                                 var4 = "NegativeBinomial",
                                                 var5 = "Weibull",
                                                 var6 = "Rank_ISR",
                                                 var7 = "Func_SharedAlpha_CS",
                                                 var8 = "Func_CS"),
                                     param = list(z_class = list(stat = 1, log = 1),
                                                  var1 = list(stat = 1, log = 1),
                                                  var2 = list(stat = 1, log = 1),
                                                  var3 = list(stat = 1, log = 1),
                                                  var4 = list(stat = 1, log = 1),
                                                  var5 = list(stat = 1, log = 1),
                                                  var6 = list(mu = list(stat = 1, log = 1),
                                                              pi = list(stat = 1, log = 1)),
                                                  var7 = list(alpha = list(stat = 1, log = 1),
                                                              beta = list(stat = 1, log = 1),
                                                              sd = list(stat = 1, log = 1)),
                                                  var8 = list(alpha = list(stat = 1, log = 1),
                                                              beta = list(stat = 1, log = 1),
                                                              sd = list(stat = 1, log = 1))),
                                     data = list(z_class = list(completed = 1:10),
                                                 var1 = list(a = 1),
                                                 var2 = list(a = 1))))
  
  class(expectedRes) = "MixtComp"
  
  resOut <- reduceRMixtCompOutput(res, completed = TRUE, log = FALSE)
  
  expect_equal(resOut, expectedRes)


  res2 <- list(variable = res$variable, nClass = 2:3, res = list(NULL, res, res))
  class(res2) = "MixtCompLearn"
  
  expectedRes2 <- list(variable = expectedRes$variable, nClass = 2:3, res = list(NULL, expectedRes, expectedRes))
  resOut <- reduceRMixtCompOutput(res2, completed = TRUE, log = FALSE)
  
  expect_equivalent(resOut, expectedRes2)
  
  
})




test_that("reduceRMixtCompOutput works in FALSE TRUE", {
  res = list(variable = list(type = list(z_class = "LatentClass",
                                         var1 = "Gaussian",
                                         var2 = "Multinomial",
                                         var3 = "Poisson",
                                         var4 = "NegativeBinomial",
                                         var5 = "Weibull",
                                         var6 = "Rank_ISR",
                                         var7 = "Func_SharedAlpha_CS",
                                         var8 = "Func_CS"),
                             param = list(z_class = list(stat = 1, log = 1),
                                          var1 = list(stat = 1, log = 1),
                                          var2 = list(stat = 1, log = 1),
                                          var3 = list(stat = 1, log = 1),
                                          var4 = list(stat = 1, log = 1),
                                          var5 = list(stat = 1, log = 1),
                                          var6 = list(mu = list(stat = 1, log = 1),
                                                      pi = list(stat = 1, log = 1)),
                                          var7 = list(alpha = list(stat = 1, log = 1),
                                                      beta = list(stat = 1, log = 1),
                                                      sd = list(stat = 1, log = 1)),
                                          var8 = list(alpha = list(stat = 1, log = 1),
                                                      beta = list(stat = 1, log = 1),
                                                      sd = list(stat = 1, log = 1))),
                             data = list(z_class = list(completed = 1:10),
                                         var1 = list(completed = 1:10, a = 1),
                                         var2 = list(time = 1:10, data = 1, a = 1))))
  
  class(res) = "MixtComp"
  
  expectedRes = list(variable = list(type = list(z_class = "LatentClass",
                                                 var1 = "Gaussian",
                                                 var2 = "Multinomial",
                                                 var3 = "Poisson",
                                                 var4 = "NegativeBinomial",
                                                 var5 = "Weibull",
                                                 var6 = "Rank_ISR",
                                                 var7 = "Func_SharedAlpha_CS",
                                                 var8 = "Func_CS"),
                                     param = list(z_class = list(stat = 1),
                                                  var1 = list(stat = 1),
                                                  var2 = list(stat = 1),
                                                  var3 = list(stat = 1),
                                                  var4 = list(stat = 1),
                                                  var5 = list(stat = 1),
                                                  var6 = list(mu = list(stat = 1),
                                                              pi = list(stat = 1)),
                                                  var7 = list(alpha = list(stat = 1),
                                                              beta = list(stat = 1),
                                                              sd = list(stat = 1)),
                                                  var8 = list(alpha = list(stat = 1),
                                                              beta = list(stat = 1),
                                                              sd = list(stat = 1))),
                                     data = list(z_class = list(completed = 1:10),
                                                 var1 = list(completed = 1:10, a = 1),
                                                 var2 = list(time = 1:10, data = 1, a = 1))))
  
  class(expectedRes) = "MixtComp"
  
  resOut <- reduceRMixtCompOutput(res, completed = FALSE, log = TRUE)
  
  expect_equal(resOut, expectedRes)
  
  
  res2 <- list(variable = res$variable, nClass = 2:3, res = list(NULL, res, res))
  class(res2) = "MixtCompLearn"
  
  expectedRes2 <- list(variable = expectedRes$variable, nClass = 2:3, res = list(NULL, expectedRes, expectedRes))
  resOut <- reduceRMixtCompOutput(res2, completed = FALSE, log = TRUE)
  
  expect_equivalent(resOut, expectedRes2)
  
  
})


test_that("computeDiscrimPowerVar works", {
  
  output <- list(mixture = list(IDClass = matrix(1:10, nrow = 2),
                                IDClassBar = matrix(1:10, nrow = 2)[2:1,]))
  
  out <- computeDiscrimPowerVar(output)
  expect_equal(out, c(-2, -6, -10, -14, -18))
  
  out <- computeDiscrimPowerVar(output, 1)
  expect_equal(out, 1 - output$mixture$IDClass[1,] - output$mixture$IDClass[2,])
  
  expect_error(out <- computeDiscrimPowerVar(output, 10))
  
})
