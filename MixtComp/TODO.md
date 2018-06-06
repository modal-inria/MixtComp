# Current

# Short Term

- The current estimator of SEM based on median of estimators at each iteration is not robust.
	- A parameter can be multivalued. When the individual medians are combined, the distribution obtained could potentially be very different from the every distribution observed during the SEM.
	- The same problem occured with the categorical proportions. The solution was to normalize the estimated parameters so that the coefficients sum to 1.

# Algorithm

- in IMixture::computeObservedProba, the number of samplings used are determined by constants, but they should increase when the complexity of the model increases


# Build

- would it be wise to split MixtComp in a base package which only requires Rccp, and a companion package which requires plotly and others ?

# Input / Output

- Gaussian: return statistics about completed data as a matrix instead of a list of vector 
- Poisson / Ordinal: return statistics about completed data as a matrix instead of a list of vector

# Model

## SimpleMixtureBridge

- Uncomment and correct SimpleMixtureBridge utest

## Functional

- Missing data in support
- Estimation has an infinite number of solution if subregressions are not separated: check for that ?
- in Predict mode with functional data, the logStorage_ (defined in mixt_ConfIntParamStat.h) isn't initialized in setParamStorage() (defined in mixt_ConfIntParamStat.h), unlike statStorage_ .. But exportDataParam() (defined in mixt_FunctionalMixture.h) tries to get it nonetheless : what's the role of logStorage_ in Predict Mode ? should it be initialized or should exportDataParam() be modified ?

## Rank

- The identifiability of the model is not enforced. Opposite ranks with symmetric values (around 0.5) of dispersion would yield the same model.

## Ordinal model

- apparently there are sometimes errors in the unit tests. Check those as they could explain other errors.
- debug all utest in UTestOrdinal.cpp
- tests for Ordinal model in RMixtComp are disabled

# Performances

- remove regex in data parsing. Will be faster but less tolerant to data format.
- Statistics object generate a boost::variate_generator each time a variable is sampled. It should be possible to generate a vector of value, when the parameters do not vary.
- If a class is emptied, the run should continue with one less class
    - there should be a way to deactivate a class, that would be simpler than moving / resizing everything

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
    - add IMixture::pushParametersToCache and pullParametersFromCache methods (it's complicated)
- this way, the data / param export at the end of the Gibbs is not affected