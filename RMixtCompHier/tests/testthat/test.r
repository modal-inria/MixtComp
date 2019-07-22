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


