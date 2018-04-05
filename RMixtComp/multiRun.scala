#!/bin/sh
exec scala "$0" "$@"
!#

import scala.language.postfixOps
import sys.process._

val nRunMax = 100

val status = Process("src/test/test > log 2>&1", None, "MC_DETERMINISTIC" -> "T") !

if (status != 0) {
  println("Error in run.")
} else {
  println("No error in run.")
}
