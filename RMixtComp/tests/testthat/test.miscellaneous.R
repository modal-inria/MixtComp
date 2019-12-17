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

context("Miscellaneous")


test_that("extractMixtCompObject works", {
  resLearn <- list(algo = list(nClass = 2), nClass = 2:5, 
                   res = list(list(algo = list(nClass = 2)),
                              list(algo = list(nClass = 3)),
                              list(algo = list(nClass = 4)),
                              list(algo = list(nClass = 5))))
  class(resLearn) = c("MixtCompLearn", "MixtComp")
  for(i in seq_along(resLearn$res))
    class(resLearn$res[[i]]) = "MixtComp"
  
  out <- extractMixtCompObject(resLearn, 2)
  expect_equal(class(out), "MixtComp")
  expect_equal(out$algo$nClass, 2)

  out <- extractMixtCompObject(resLearn)
  expect_equal(class(out), "MixtComp")
  expect_equal(out$algo$nClass, resLearn$algo$nClass)

  expect_error(extractMixtCompObject(resLearn, 3.5))
  expect_error(extractMixtCompObject(resLearn, 2:3))
  expect_error(extractMixtCompObject(5))
})

