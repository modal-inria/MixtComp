#!/usr/bin/python

import re
import subprocess

nbRunMax = 100

pattern = re.compile(r"lnCompletedLikelihood.*")


for i in range(nbRunMax):
    print '%d / %d' % (i + 1, nbRunMax)
    subprocess.call(["src/test/test > log 2>&1"], shell=True) # potential security flaw: https://docs.python.org/2/library/subprocess.html#frequently-used-arguments
    lastLine = subprocess.check_output(['tail', '-1', 'log']).rstrip()
    if not pattern.match(lastLine):
        print "faulty run detected"
        break
