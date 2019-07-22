# MixtComp Libraries and packages

## French

**MixtComp** est écrit en C++ et a besoin de deux librairies externes pour fonctionner :
- **Eigen** (http://eigen.tuxfamily.org), libraire d'algèbre linéaire, sous licence MPL2
- **Boost** (https://www.boost.org/), une librarie C++ sous licence Boost (https://www.boost.org/users/license.html)
- **CppOptimizationLibrary** (https://github.com/PatWie/CppNumericalSolvers) une librairie fournissant des solveurs sous licence MIT.

Afin d'être appelé par différents languages, différentes interfaces ont été créées :
- **JMixtComp**, fournissant un exécutable C++ avec des fichiers json en entrées/sorties, dépendant de la librairie **nlohmann/json** (https://github.com/nlohmann/json) sous licence MIT.
- **RJMixtComp**, package R contenant l'exécutable *JMixtComp* et permettant de créer les fichiers json d'entrées, de lancer l'exécutable JMixtComp depuis R et de lire les sorties json en R
- **RMixtCompIO**, package R interfaçant MixtComp, il utilise le package R/librairie C++ **Rcpp** (https://github.com/RcppCore/Rcpp) sous licence GPL (>= 2). Il contient des fichiers C++ utilisant la librairie *Rcpp* pour convertir les données du format R vers les formats standards de C++

Les fonctions C++ de *MixtComp* et *JMixtComp* sont testées en utilisant la librairie **GoogleTest** (https://github.com/google/googletest) sous licence BSD3.

Des packages R complémentaires existent :
- **RMixtCompUtilities**, package en R contenant des fonctions graphiques, de formatage de données et des getters pour analyser les résultats des sorties *MixtComp*
- **RMixtComp**, package en R chargeant *RMixtCompUtilities* et *RMixtCompIO*
- **RMixtCompHier**, package en R implémentant une version hiérarchique de *MixtComp* (utilisée sur le contrat Alstom). Le package contient un ancien exécutable de *JMixtComp* (approximativement de la V2 de *MixtComp*) utilisé pour le contrat Alstom. Le package charge également *RJMixtComp* afin de pouvoir appliqué la même stratégie avec la version la plus récente de *MixtComp*.


## English

**MixtComp** is written in C++ and requires two external libraries to work:
- **Eigen** (http://eigen.tuxfamily.org), a linear algebra library, under MPL2 licence
- **Boost** (https://www.boost.org/), a C++ library under Boost licence (https://www.boost.org/users/license.html)
- **CppOptimizationLibrary** (https://github.com/PatWie/CppNumericalSolvers) a C++ library providing solvers under MIT licence.

In order to be called by different languages, different interfaces have been created:
- **JMixtComp**, providing a C++ executable with json files as input/output files. It depends on the **nlohmann/json** library (https://github.com/nlohmann/json) under MIT licence.
- **RJMixtComp**, an R package containing the executable *JMixtComp* and allowing to create the json input files, to launch the executable *JMixtComp* from R and to read the json output in R
- **RMixtCompIO**, an R package interfacing *MixtComp*, it uses the R package/C++ library **Rcpp** (https://github.com/RcppCore/Rcpp) under GPL (>= 2) licence. It contains C++ files using the *Rcpp* library to convert data from R format to standard C++ formats

The C++ functions of *MixtComp* and *JMixtComp* are tested using the **GoogleTest** library (https://github.com/google/googletest) under BSD3 licence.

Additional R packages exist:
- **RMixtCompUtilities**, an R package containing getter, formatting, graphical functions to analyze the results of *MixtComp* outputs
- **RMixtComp**, an R package loading *RMixtCompUtilities* and *RMixtCompIO*
- **RMixtCompHier**, an R package implementing a hierarchical version of *MixtComp* (used on the Alstom contract). The package contains an old *JMixtComp* executable (approximately from *MixtComp*'s V2) used for the Alstom contract. The package also loads *RJMixtComp* in order to be able to apply the same strategy with the most recent version of *MixtComp*.



