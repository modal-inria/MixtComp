# Vraisemblance complétée

- calcul du cache affecté

## Short Term

- The current estimator of SEM based on median of estimators at each iteration is not robust.
  - A parameter can be multivalued. When the individual medians are combined, the distribution obtained could potentially be very different from the every distribution observed during the SEM.
  - The same problem occurred with the categorical proportions. The solution was to normalize the estimated parameters so that the coefficients sum to 1.

## Algorithm

- in IMixture::computeObservedProba, the number of samplings used are determined by constants, but they should increase when the complexity of the model increases

## Input / Output

- Gaussian: return statistics about completed data as a matrix instead of a list of vector
- Poisson / Ordinal: return statistics about completed data as a matrix instead of a list of vector

## Model

- SimpleMixtureBridge
  - Uncomment and correct SimpleMixtureBridge utest
- Functional
  - Missing data in support
  - Estimation has an infinite number of solution if subregressions are not separated: check for that ?
  - in Predict mode with functional data, the logStorage_(defined in mixt_ConfIntParamStat.h) isn't initialized in setParamStorage() (defined in mixt_ConfIntParamStat.h), unlike statStorage_ .. But exportDataParam() (defined in mixt_FunctionalMixture.h) tries to get it nonetheless : what's the role of logStorage_ in Predict Mode ? should it be initialized or should exportDataParam() be modified ?
- Rank
  - The identifiability of the model is not enforced. Opposite ranks with symmetric values (around 0.5) of dispersion would yield the same model.

## Performances

- remove regex in data parsing. Will be faster but less tolerant to data format.
- Statistics object generate a boost::variate_generator each time a variable is sampled. It should be possible to generate a vector of value, when the parameters do not vary.

## Long Term

- données gaussiennes type "spike and slab"
  - on demande un niveau d'imprécision à l'utilisateur et on remplace par un intervalle
  - fonctionnelle constante : on demande un niveau d'imprécision à l'utilisateur et on remplace par un intervalle
