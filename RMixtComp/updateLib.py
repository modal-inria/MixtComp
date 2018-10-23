#! /usr/local/bin/python3

import glob
import subprocess
import sys

if sys.version_info[0] < 3 and sys.version_info[1] < 5:
    raise Exception("Python 3.5 or a more recent version is required.")

print("Note that updateLib.py updates the src/lib subdirectory. To update the optim directory, get the sources from https://github.com/PatWie/CppNumericalSolvers and directly commit a new version.")

libSource = "../MixtComp/src/lib"

subprocess.call(["rsync", "-a", "--delete", libSource, "src"])

cppList = glob.iglob("src/lib/**/*.cpp", recursive=True)
oList = [filename[4:-3] + 'o' for filename in cppList]
oListStr = ' '.join(oList)
fileContent = "LIB_OFILES = " + oListStr

text_file = open("src/libOFiles", "w")
text_file.write(fileContent)
text_file.close()
