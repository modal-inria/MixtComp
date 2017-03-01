# Discussion
- deux types de dégénérescence, vraisemblance bornée ou pas
- dans l'algo, pas de bornes, rajouter les bornes avant l'export de paramètres
- si en prédiction, avec bornes partout, la variable devient muette
- Est-ce qu'il ne vaudrait pas mieux initialiser les paramètres en tirant des individus, pour aller plus vite
- Initialiser en générant une liste de z
    - soit le modèle est capable d'estimer à partir d'un individu, et c'est le centre de la classes
    - soit il peut pas (multinomial), et dans ce cas, il utilise toute la liste pour initialiser comme actuellement
    - au pire des cas on est dans le cas actuel, et au mieux, on explore mieux l'espace
- Si l'algo est plus rapide, on peut intégrer les initalisations multiples, et renvoyer à l'utilisateur seulement le meilleur.
- Sortir les vraisemblances individuelles des individus, pour éviter d'avoir à lancer des prédictions à un individu

# Various

- Remove all licence information from all files. Curently there is a licence header.

# Performances

- Computing log observed likelihood takes a lot of time. Are we sure that it is only computed once ?
    - needed to compute BIC and ICL
    - to compute IDClass
    - to compute lnProbaGivenClass
- Is it at least cached for the variables for which it takes a lot of time ?
- Now that lnProbaGivenClass computes the detail of the log proba, its result could be cached and used in all the cases listed
    - in learn, it should be called at the end of the SEM
    - in predict, it should be called at the beginning of the Gibbs, once the parameters are known
    - this centralize the caching in the composer, instead of delegating it to the mixture author

# Build

- Nlopt should be built the same way boost regex is built. This would ensure a proper integration in the build system, and a faster build.
- There are a lot of complicated dependencies, which make mixtcomp hard to build on a system without root access
    - would it be wise to split MixtComp in a base package which only requires Rccp, and a companion package which requires plotly and others ?
- CC and CFLAGS variables should be exported, so that alternative C compilers could be provided the same way C++ compilers are. This could be useful for packages like nlopt
    - this should be added to jsonmixtcomp
- Move all external libraries to a specific folder, to clearly separate them from the various MixtComp modules

# Bugs

- Functional model seem to crash when edge authorized option is activated for the estimators
    - They do not support biased estimators and since the Gibbs is deactivated, null variance or similar errors could happen
- parameterEdgeAuthorized limits the estimation by excluding the border of the parameter space
	- if a class is empty, its proportion will be estimated at 1e-8. BUT the estimation of the classes will not work as there are no observations. Crashes are to be expected.
- Uncomment and correct SimpleMixture utest

# Features

- Switch to Gitlab
- Put in place continuous integration
    - Add compilation flag to set random seed, to allow reproducibility of stochastic tests

# Estimation problem
- keep the test for degenerescency
- remove the Gibbs
    - how will the Ordinal model work without Gibbs ?
    - it will not work.
- remove parameterEdgeAuthorized
    - it "worked" for problem of null variance and such
    - it never worked for empty classes
    - remove it altogether ?
- launch multiple run if a degenerescency is detected
- SEM will degenerate given enough time, so if the data is "correct", this should be rare, and fixed by relaunching
- if degenerescency is systematic, the model is not correct and should be dropped. Running a Gibbs over a long time period will not solve anything.

# Output object
- Gaussien :
    Les statistiques des données complétées au format matriciel
- Poisson :
    Les statistiques des données complétées au format matriciel
- Ordinal :
    Les statistiques des données complétées sont manquantes
- zclass
    data\$z_class\$stat ajouter des noms de colonnes au tik

# Model

- Missing data in support in Functional Model
- Ordinal model
    - clarify if can be used with EdgeAuthorized or not. It seems it cannot. Test with generated data.
- remove regex in data parsing. Will be faster but less tolerant to data format.

# R

- Export data in functional Model
- Refactor RMixtComp package files (R subdirectory)
- Clean the Makefile used in mcUpdate, so that it become similar to what is done in jsonmixtcomp and vanilla mixtcomp

# Json

- Output rows names as a vector to easily recreate arrays
- In input, separate
    - strategy and number of classes
    - descriptor
    - this way you can edit the strategy and model parameters without editing the data file
- in predict mode, can't read file path with : or ~
- exportVals function for functional data
- delta manquant dans $mixture
- IDClass est transposée par rapport à la sortie de RMixtComp
- Rank
    Problème de lecture des données de rangs (le même jeu de données marche avec RMixtComp)
- Functional
    Problème de lecture des données fonctionnelles (le même jeu de données marche avec RMixtComp)
- Categorical
    données complété stat : retourne 0.52 0.52 au lieu d'une liste [[1]] 2 [[2]] 0.52
    dans param\$categorical1\$NumericalParam\$stat est sous format de liste et non de matrice
    dans param\$categorical1\$NumericalParam\$log est sous format de liste et non de matrice
- Poisson
    dans param\$poisson1\$NumericalParam\$log est sous format de liste et non de matrice
    dans param\$poisson1\$NumericalParam\$stat est sous format de liste et non de matrice
- Ordinal
    données complétées décalées tt de 1
    dans param\$Ordinal1\$muPi\$log est sous format de liste et non de matrice
    dans param\$Ordinal1\$muPi\$stat est sous format de liste et non de matrice
- z_class
    dans param\$z_class\$pi\$stat est sous format de liste et non de matrice
    dans param\$z_class\$pi\$log est sous format de liste et non de matrice
- gaussian
    dans param\$gaussian1\$NumericalParam\$stat est sous format de liste et non de matrice
    dans param\$gaussian1\$NumericalParam\$log est sous format de liste et non de matrice

# Visualization and post-processing

# Long term

- co-clustering
- parallel Spark
