# Current

# Short Term

- reactivate all utest in UTestOrdinal.cpp
- The current estimator of SEM based on median of estimators at each iteration is not robust.
	- A parameter can be multivalued. When the individual medians are combined, the distribution obtained could potentially be very different from the every distribution observed during the SEM.
- Check that observed probabilities are not nan in the failing functional case
- functional, check if there are enough different values in t

# Algorithm

- in IMixture::computeObservedProba, the number of samplings used are determined by constants, but they should increase when the complexity of the model increases
- What to do when an individual is not suited for initialization ?
    - for example when a functional does not have enough points to estimate all the subregressions ?
    - those are extreme cases that at present would result in a crash
- The initialization sequence uses one individual per class. In semi-supervized problems one must ensure that the constraints are applied and the selected individual can belong to the class.

# Bugs

- Functional model seem to crash when edge authorized option is activated for the estimators
    - They do not support biased estimators and since the Gibbs is deactivated, null variance or similar errors could happen
- Crashes observed when running testOrdinal() in RMixtComp

# Build

- Nlopt should be built the same way boost regex is built, that is, not using the makefile provided by nlopt. This would ensure a proper integration in the build system, and a faster build.
- There are a lot of complicated dependencies, which make mixtcomp hard to build on a system without root access
    - would it be wise to split MixtComp in a base package which only requires Rccp, and a companion package which requires plotly and others ?
- CC and CFLAGS variables should be exported, so that alternative C compilers could be provided the same way C++ compilers are. This could be useful for packages like nlopt
    - this should be added to jsonmixtcomp
- Move all external libraries to a specific folder, to clearly separate them from the various MixtComp modules

# Input / Output

- Gaussian: Les statistiques des données complétées au format matriciel (liste de vecteurs en ce moment)
- Poisson / Ordinal: Les statistiques des données complétées au format matriciel (liste de vecteurs en ce moment)

# Long Term

- données gaussiennes type "spike and slab"
    - on demande un niveau d'imprécision à l'utilisateur et on remplace par un intervalle
    - fonctionnelle constante : on demande un niveau d'imprécision à l'utilisateur et on remplace par un intervalle
- Mode sans échec :
    - Il va modifier les données en accord avec l'utilisateur. Il reprend les logs de MixtComp et pose des questions à l'utilisateur pour créer un jeu de données qui va mieux pour MixtComp.
    - L'utilisateur devra par exemple rentrer un niveau d'imprécision qu'il tolère sur la mesure de données. Si on ne peut pas prédire un individu, on propose de mettre des manquants aux variables qui gènent pour cet individu.
- launch N identical runs, compare the log observed likelihood and keep the better one
    - how to keep all the parameters and of the last best run ?
    - note that only the SEM phase has to be run n times, not the Gibbs sampling
    - each mixture has to be responsible for pushing / pulling the best parameters from a cache
    - add IMixture::pushParametersToCache and pullParametersFromCache methods
- this way, the data / param export at the end of the Gibbs is not affected

# Model

## SimpleMixtureBridge

- Uncomment and correct SimpleMixtureBridge utest

## Functional

- Missing data in support
- Estimation has an infinite numer of solution if subregressions are not separated: check for that ?
- in Predict mode with functional data, the logStorage_ (defined in mixt_ConfIntParamStat.h) isn't initialized in setParamStorage() (defined in mixt_ConfIntParamStat.h), unlike statStorage_ .. But exportDataParam() (defined in mixt_FunctionalMixture.h) tries to get it nonetheless : what's the role of logStorage_ in Predict Mode ? should it be initialized or should exportDataParam() be modified ?
- FunctionalClass::checkNbDifferentValue might not be powerful enough for the detection due to floating point arithmetics
- FunctionalClass::checkNonNullSigma should use a relative epsilon juste like Gaussian to check for variability
    - one way to easily deal with this should be to have mStep return a string, if the check is as complicated as an estimation...

## Rank

- The identifiability of the model is not enforced. Opposite ranks with symmetric values (around 0.5) of dispersion would yield the same model.

## Ordinal model

- clarify if can be used with EdgeAuthorized or not. It seems it cannot. Test with generated data.
- apparently there are sometimes errors in the unit tests. Check those as they could explain other errors.

# Performances

- remove regex in data parsing. Will be faster but less tolerant to data format.
- Statistics object generate a boost::variate_generator each time a variable is sampled. It should be possible to generate a vector of value, when the parameters do not vary.

- If a class is emptied, the run should continue with one less class
    - there should be a way to deactivate a class, that would be simpler than moving / resizing everything

données gaussiennes type "spike and slab" : on demande un niveau d'imprécision à l'utilisateur et on remplace par un intervalle
fonctionnelle constante : on demande un niveau d'imprécision à l'utilisateur et on remplace par un intervalle

# Various

- Since the algorithm is stochastic, it is complicated to obtain reproducible results during unit testing and continuous integration.
	- If the seeds of all RNG are fixed to the same value, then there is not enough variability in the generated data for the test to pass. Should the data be generated normally ?
	- If rng are shared, then there will be race conditions in multi-threading runs
	- So the deterministic approach is put on standby. mixt_RNG.h contains the seed generation.
	- This also highlight the problem that the algorithm is dependent on the initialization of the RNG and this should be dealt with...