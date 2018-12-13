# MixtComp
MixtComp (Mixture Composer) is a model-based clustering package for mixed data originating from the [Modal team](https://modal.lille.inria.fr/wikimodal/doku.php) (Inria Lille).

It has been engineered around the idea of easy and quick integration of all new univariate models, under the conditional independence assumption. New models will eventually be available from researches, carried out by the Modal team or by other teams. Currently, central architecture of MixtComp is built and functionality has been field-tested through industry partnerships. Five basic models (Gaussian, Multinomial, Poisson, Weibull, Negative Binomial) are implemented, as well as two advanced models (Functional and Rank).

MixtComp has the ability to natively manage missing data (completely or by interval). MixtComp is used as an R package, but its internals are coded in C++ using state of the art libraries for faster computation.

Online version: [MASSICCC](https://massiccc.lille.inria.fr/)

# Code organization

* **MixtComp** MixtComp C++ library
* **RMixtComp** R package linking MixtComp with Rcpp (internal use)
* **RJMixtComp** R package using JMixtComp (for customers)
* **MixtCompRTools** R package containing the hierarchical MixtComp

# Documentation

* [Compile MixtComp](MixtComp/README.md)
* [Install RMixtComp](RMixtComp/README.md)
* [Data format](MixtComp/docs/dataFormat.md)
* [Output object](MixtComp/docs/objectOutput.md)
* [Algorithm description](MixtComp/docs/algoDesc.md)
* [Add a model](MixtComp/docs/howToAddModel.md)
* [Add a model in R packages](MixtComp/docs/howToAddModelInR.md)
* [Styleguide](MixtComp/docs/styleguide.md)

# External tools

* [Trello](https://trello.com/b/vTK9pUM4/mixtcomp)
* [Mailing-list](https://sympa.inria.fr/sympa/info/mixtcomp-dev)
* [Continuous integration server](https://ci.inria.fr/mixtcomp/)

# Branches

There are two branches tested on the ci server (using the [ci.sh](ci.sh) file):
* **master** this branch is protected, MixtComp must *always* work on it 
* **staging** this branch is used for short development, testing new features, bug fixes... and its content is regularly pushed to master when tests are OK.
Big features should be developed on a specific branch.