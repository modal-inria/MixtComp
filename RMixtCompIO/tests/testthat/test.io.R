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


# @author Vincent Kubicki
context("IO")

test_that("add_payload", {
  input <- list(
    a = list(
      a1 = "riri",
      a2 = "fifi",
      a3 = "loulou"
    ),
    b = list(
      b1 = list(
        b11 = "dingo"
      )
    )
  )

  output <- UTest1()

  expect_identical(input, output)
})

test_that("exist_payload", {
  output <- UTest2()

  expect_identical(TRUE, output)
})

test_that("exist_payload, negative case", {
  output <- UTest6()

  expect_identical(FALSE, output)
})

test_that("get_payload", {
  output <- UTest3()

  expect_identical("riri", output)
})

test_that("NumericVector IO", {
  l <- list()
  l$This$is$Sparta <- c(a = 12, b = 12, c = 42.5)

  output <- UTest4(l)

  expect_identical(l, output)
})

test_that("NumericMatrix IO", {
  l <- list()
  l$mat <- matrix(
    c(1, 2, 3, 4, 5, 6),
    nrow = 2,
    dimnames = list(
      c("row1", "row2"),
      c("col1", "col2", "col3")
    ) # column names )
  )

  output <- UTest5(l)

  expect_identical(l, output)
})
