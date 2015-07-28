#!/usr/bin/python

import subprocess

nbRunMax = 100
nbFail = 0

for i in range(nbRunMax):
    print '%d / %d' % (i + 1, nbRunMax)
    subprocess.call(["src/test/test > log 2>&1"], shell=True) # potential security flaw: https://docs.python.org/2/library/subprocess.html#frequently-used-arguments
    lastLine = subprocess.check_output(['tail', '-1', 'log']).rstrip()
    if lastLine == "lnCompletedLikelihood:":
        print "faulty run detected"
        break
