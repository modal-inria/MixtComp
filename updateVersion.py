#! /usr/local/bin/python3
import os

def changeHeader(directory, startPatternVersion, newLineVersion, startPatternCopyright, newLineCopyright, extension = (".h", ".cpp")):
	for filename in os.listdir(directory):
		if os.path.isdir(directory + "/" + filename):
			changeHeader(directory + "/" + filename, startPatternVersion, newLineVersion, startPatternCopyright, newLineCopyright, extension)
		elif filename.endswith(extension):
			fullName = directory + '/' + filename
			with open(fullName) as f:
				lines = f.readlines()
				toChange = False
				if len(lines) > 1:
					if lines[0].startswith(startPatternVersion):
						lines[0] = newLineVersion
						toChange = True
					if lines[1].startswith(startPatternCopyright):
						lines[1] = newLineCopyright
						toChange = True
					if toChange:
						with open(fullName, "w") as f:
							f.writelines(lines)
					else:
						print(directory + "/" + filename)


# change license cpp files
startPatternVersion = "/* MixtComp "
newLineVersion = "/* MixtComp version 4  - july 2019\n"
startPatternCopyright = " * Copyright"
newLineCopyright = " * Copyright (C) Inria - Université de Lille - CNRS */\n"
extension = (".h", ".cpp")

directories = ["MixtComp/src/lib/", "MixtComp/src/utest/", "MixtComp/src/json/"]


for directory in directories:
	changeHeader(directory, startPatternVersion, newLineVersion, startPatternCopyright, newLineCopyright, extension)


# change license R files
startPatternVersion = "# MixtComp "
newLineVersion = "# MixtComp version 4  - july 2019\n"
startPatternCopyright = "# Copyright"
newLineCopyright = "# Copyright (C) Inria - Université de Lille - CNRS\n"
extension = (".R", ".r")

directories = ["RMixtCompIO", "RMixtCompHier", "RMixtCompUtilities", "RMixtComp", "RJMixtComp"]

for directory in directories:
	changeHeader(directory, startPatternVersion, newLineVersion, startPatternCopyright, newLineCopyright, extension)



# change license R files
startPatternVersion = "/* MixtComp "
newLineVersion = "/* MixtComp version 4  - july 2019\n"
startPatternCopyright = " * Copyright"
newLineCopyright = " * Copyright (C) Inria - Université de Lille - CNRS */\n"
extension = (".h", ".cpp")

directories = ["RMixtCompIO/src"]

for directory in directories:
	changeHeader(directory, startPatternVersion, newLineVersion, startPatternCopyright, newLineCopyright, extension)

