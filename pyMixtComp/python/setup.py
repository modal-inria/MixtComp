from setuptools import setup

setup(
    name="pyMixtComp",
    version="0.1",
    author="Quentin Grimonprez",
    description="Mixture models with heterogeneous data sets and partially missing data management.",
    long_description=open("README.md", "r").read(),
    long_description_content_type="text/markdown",
    keywords=["clustering", "mixture model", "heterogeneous", "missing data"],
    classifiers=[
        "Programming Language :: C++",
        "Programming Language :: Python",
        "Topic :: Scientific/Engineering :: Artificial Intelligence"
    ],
    packages=["pyMixtComp"],
    include_package_data=True,
    package_data={"pyMixtComp": ["pyMixtCompBridge.so"], "": ["data/*.csv"]},
    install_requires=["numpy", "pandas", "scikit-learn"],
)
