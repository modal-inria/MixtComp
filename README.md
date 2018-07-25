# MixtComp
MixtComp (Mixture Composer) is a model-based clustering package for mixed data originating from the [Modal team](https://modal.lille.inria.fr/wikimodal/doku.php) (Inria Lille). 

It has been engineered around the idea of easy and quick integration of all new univariate models, under the conditional independence assumption. New models will eventually be available from researches, carried out by the Modal team or by other teams. Currently, central architecture of MixtComp is built and functionality has been field-tested through industry partnerships. Five basic models (Gaussian, Multinomial, Poisson, Weibull, Negative Binomial) are implemented, as well as three advanced models (Fucntional, Ordinal and Rank). 

MixtComp has the ability to natively manage missing data (completely or by interval). MixtComp is used as an R package, but its internals are coded in C++ using state of the art libraries for faster computation. 

Online version: [MASSICCC](https://massiccc.lille.inria.fr/)

# Code organization

* **MixtComp** MixtComp C++ library
* **JsonMixtComp** Use MixtComp with json files input and output (for customers)
* **RMixtComp** R package linking MixtComp with Rcpp (internal use)
* **RMixtCompPlot** R package with graphical functions
* **RJsonMixtComp** R package using JsonMixtComp (for customers)
* **libraries** external libraries and cmake scripts

# Documentation

* [Compile MixtComp](./MixtComp/README.md)
* [Compile JsonMixtComp](./JsonMixtcomp/README.md)
* [Install RMixtComp](./RMixtComp/README.md)
* [Data format](./MixtComp/docs/dataFormat.md)
* [Output object](./MixtComp/docs/objectOutput.md)
* [Algorithm description](./MixtComp/docs/algoDesc.md)
* [Statistical model description](./MixtComp/docs/howToAddModel.md)
* [Add a model in R packages](./MixtComp/docs/howToAddModelInR.md)