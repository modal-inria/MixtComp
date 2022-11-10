from setuptools import setup

import re
import sys

# to ensure that we error out properly for people with outdated setuptools
# and/or pip.
py_min_version = (3, 9)  # minimal supported python version

if sys.version_info < py_min_version:
    error = f""" Python {py_min_version} or above is required. You are using Python {sys.version_info[:3]}."""


VERSIONFILE = "pyMixtComp/_version.py"
verstrline = open(VERSIONFILE, "rt").read()
VSRE = r"^__version__ = ['\"]([^'\"]*)['\"]"
mo = re.search(VSRE, verstrline, re.M)
if mo:
    verstr = mo.group(1)
else:
    raise RuntimeError("Unable to find version string in %s." % (VERSIONFILE,))

setup(
    name="pyMixtComp",
    version=verstr,  # version number is set in pyMixtComp/_version.py
    author="Quentin Grimonprez",
    description="Mixture models with heterogeneous data sets and partially missing data management.",
    long_description=open("README.md", "r").read(),
    long_description_content_type="text/markdown",
    keywords=["clustering", "mixture model", "heterogeneous", "missing data"],
    classifiers=[
        "Programming Language :: C++",
        'Programming Language :: Python :: 3.9',
        'Programming Language :: Python :: 3.10',
        'Programming Language :: Python :: 3.11',
        "Topic :: Scientific/Engineering :: Artificial Intelligence"
    ],
    packages=["pyMixtComp"],
    include_package_data=True,
    package_data={"pyMixtComp": ["pyMixtCompBridge.so"], "": ["data/*.csv"]},
    python_requires='>={}'.format('.'.join(str(n) for n in py_min_version)),
    install_requires=[
        "numpy==1.23.4",
        "pandas==1.5.1",
        "scikit-learn==1.1.3",
        "scipy==1.9.3",
        "matplotlib==3.6.2",
        "seaborn==0.12.1",
        ],
)
