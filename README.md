# MixtComp

MixtComp (Mixture Composer) is a model-based clustering package for mixed data originating from the [Modal team](https://modal.lille.inria.fr/wikimodal/doku.php) (Inria Lille).

Mixture models parameters are estimated using a [SEM algorithm](MixtComp/docs/article/SEM.pdf). Five basic models (Gaussian, Multinomial, Poisson, Weibull, NegativeBinomial) are implemented to manage real, integer and categorical variables, as well as two advanced models ([Func_CS](MixtComp/docs/article/functionalModel.pdf) for functional data and [Rank_ISR](MixtComp/docs/article/rankModel.pdf) for rank data). MixtComp has the ability to natively manage missing data (completely or by interval). 

MixtComp is used as an R package, but its internals are coded in C++ using state of the art libraries for faster computation. It has been engineered around the idea of easy and quick integration of all new univariate models, under the conditional independence assumption. New models will eventually be available from researches, carried out by the Modal team or by other contributors. Currently, central architecture of MixtComp is built and functionality has been field-tested through industry partnerships. 

Online version (not up-to-date): [MASSICCC](https://massiccc.lille.inria.fr/).

CRAN package: [![CRAN_Status_Badge](http://www.r-pkg.org/badges/version/RMixtComp)](https://cran.r-project.org/package=RMixtComp)
[![Total Downloads](http://cranlogs.r-pkg.org/badges/grand-total/RMixtComp?color=blue)](http://cranlogs.r-pkg.org/badges/grand-total/RMixtComp)

## Credits

The following people contributed to the development of MixtComp: Vincent Kubicki, Christophe Biernacki, Quentin Grimonprez, Serge Iovleff, Matthieu Marbac-Lourdelle, Étienne Goffinet.

Copyrigth Inria - Université de Lille - CNRS

## Licence

MixtComp is distributed under the [AGPL 3.0 licence](https://www.gnu.org/licenses/agpl-3.0.en.html). For more details about the licences of MixtComp and its dependencies see the [LICENCE.md](LICENCE.md) file.

## Code organization

* **MixtComp** MixtComp C++ library
* **JMixtComp** C++ executable using JSON files as input/output
* **RMixtComp** Main R package loading *RMixtCompIO* and *RMixtCompUtilities*
* **RMixtCompIO** R package linking MixtComp C++ library with Rcpp
* **RMixtCompUtilities** R package containing graphical, formatting and getter functions
* **RJMixtComp** R package using a JMixtComp executable 
* **RMixtCompHier** R package containing a hierarchical version of MixtComp
* **pyMixtComp** Minimal python interface using Boost.Python

A description of the links between packages and external libraries can be found [here](architecture.md) in a text version and [here](graphArchitecture.png) in a visual version

## Documentation

* [Compile MixtComp](MixtComp/README.md)
* [Install RMixtComp](RMixtComp/README.md)
* [Data format](MixtComp/docs/dataFormat.md)
* [Output object](MixtComp/docs/objectOutput.md)
* [Algorithm description](MixtComp/docs/algoDesc.md)
* [Add a model](MixtComp/docs/howToAddModel.md)
* [Add a model in R packages](MixtComp/docs/howToAddModelInR.md)
* [Solutions to common errors](MixtComp/docs/error.md)
* [Run MixtComp in command line](JMixtComp/README.md)
* [Styleguide](MixtComp/docs/styleguide.md)

Scientific papers about algorithm and models are available in the [article folder](MixtComp/docs/article/).

## Other tools (for MixtComp dev team)

* [Trello](https://trello.com/b/vTK9pUM4/mixtcomp)
* [Mailing-list](https://sympa.inria.fr/sympa/info/mixtcomp-dev)
* [Continuous integration server](https://ci.inria.fr/mixtcomp/)

## Branches (for MixtComp dev team)

There are two branches tested on the ci server (using the [ci.sh](ci.sh) file):
* **master** this branch is protected, MixtComp must *always* work on it.
* **staging** this branch is used for short development, testing new features, bug fixes... and its content is regularly pushed to master when tests are OK.
Big features should be developed on a specific branch.
