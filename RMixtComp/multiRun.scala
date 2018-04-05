#!/bin/sh
exec scala "$0" "$@"
!#

import scala.language.postfixOps
import sys.process._

val errorStr = "Please contact the maintainer and provide your data, descriptors and lauch script."

def successfullRun(log: String): Boolean = {
  log.takeRight(errorStr.size) != errorStr
}

val nRunMax = 5

var currIt = -1
var status = 0
var success = 0
var correctRun = true

val rng = scala.util.Random
var seed = rng.nextInt

while (currIt < nRunMax && status == 0 && correctRun == true) {
  currIt += 1
  seed = rng.nextInt

  println(s"Run: $currIt, seed: $seed")

  val stdout = new StringBuilder
  val stderr = new StringBuilder
  status = Process("src/test/test", None, "MC_DETERMINISTIC" -> seed.toString) ! ProcessLogger(stdout append _, stderr append _)
  val out = stdout.toString
  println(out)
  correctRun = successfullRun(out)
}

if (status != 0 || correctRun == false) {
  println(s"Error with seed $currIt")
} else {
  println(s"No run with error.")
}
