# Réunion 24 Octobre

- Degeneracy condition that stops the algorithm
    - Empty class
    - Normal law
    - Function distribution
- IdName should be included by IMixture, not by the MixtureComposer

# Current

## SemStrategy

- should be capable of returning a boolean indicating that at least 1 run has been completed successfully
	- a string is already returned, that should be enough
	- the string might contain the result of the last run, if all were unsuccessful
- if SEMAlgo only contains a single function, that function should be part of the Strategy, no need for an additional object

## Unbounded likelihood

- even if unbounded likelihood is the only reason to abort a run, the emptying of a class should remain a reason to abort a run. It impacts every variable.
- keep checkSampleCondition, except that in the implementation, only return false for unbounded likelihood
	- the test must occur after samplingStep, to check that the completion is valid
- if parameters on the limit of domain, for example proportion = 0, what bound should be applied ?

## IMixture

- keep checkSampleCondition, except that in the implementation, only return false for unbounded likelihood
	- the test must occur after samplingStep, to check that the completion is valid
	- this is not a priority
- add pushParametersToCache and pullParametersFromCache methods
- composer.p_zi() and composer.classInd() are there in all mixture, why not include them in IMixture ?
- in IMixture::computeObservedProba, the number of samplings used are determined by constants, but they should increase when the complexity of the model increases

## Multi run

- only end the computation if the likelihood is unbound.
	- For example, not when a a proportion in a categorical model is 0.
	- end it if the standard deviation is 0
	- all those checks must be made in checkSampleCondition
- dans l'algo, pas de bornes, rajouter les bornes avant l'export de paramètres
- si en prédiction, avec bornes partout, la variable devient muette
- remove the Gibbs

## Initialization

- the mStep just after initParam will be suppressed, as it would erase the result of initParam
	- what is needed after initParam are eStep and sStep
	- it is the case in both SEMAlgo::runCheck and SEMAlgo::runNoCheck at the moment, so everything is fine
- Functional
	- How to initialize alpha ? Are there no problem to initialize it from a single individual ?
- What to do when an individual is not suited for initialization ?
	- for example when a functional does not have enough points to estimate all the subregressions ?
	- those are extreme cases that at present would result in a crash
- The uniform initialization seems useless now that one observation is used to initialize the parameters
	- is it so that only that one observation shall be uniformly completed ?
	- it is useful, because every individual need to be completed for the eStep (and not just for the mStep)
- The initialization sequence uses one individual per class. In semi-supervized problems one must ensure that the constraints are applied and the selected individual can belong to the class.

## SemStrategy

- initSEMCheck has to be removed and only initSEMNoCheck and runSEMNoCheck must be kept
- launch 10 identical runs, compare the log observed likelihood and keep the better one
	- how to keep all the parameters and of the last best run ?
	- note that only the SEM phase has to be run n times, not the Gibbs sampling
	- each mixture has to be responsible for pushing / pulling the best parameters from a cache
- this way, the data / param export at the end of the Gibbs is not affected

## Multi run

- si en prédiction, avec bornes partout, la variable devient muette
- remove the Gibbs
- export the number of degenerate run, so that an expert user could reject if there are too many

# Nomenclature

- samplingStepNoCheck -> sampleLatent
- sStep -> sampleClass

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
- remove regex in data parsing. Will be faster but less tolerant to data format.

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

# Architecture

- The whole trait system for the SimpleMixtureBridge might be greatly simplified if the mixture type, provided as a template argument of MixtureBridge, could in turn provide the types. BridgeTrait and its specialization would become unnecessary and all the information about mixture could be centralized in the header files of the various simple models.
- There is no rational to separate sStep and samplingStep, they should be merged.

# Various

- Since the algorithm is stochastic, it is complicated to obtain reproducible results during unit testing and continuous integration.
	- If the seeds of all RNG are fixed to the same value, then there is not enough variability in the generated data for the test to pass. Should the data be generated normally ?
	- If rng are shared, then there will be race conditions in multi-threading runs
	- So the deterministic approach is put on standby. mixt_RNG.h contains the seed generation.
	- This also highlight the problem that the algorithm is dependent on the initialization of the RNG and this should be dealt with...

# Input / Output

- Gaussian: Les statistiques des données complétées au format matriciel (liste de vecteurs en ce moment)
- Poisson / Ordinal: Les statistiques des données complétées au format matriciel (liste de vecteurs en ce moment)

# Model

## Simple

- Uncomment and correct SimpleMixture utest

## Functional

- Missing data in support
- Estimation has an infinite numer of solution if subregressions are not separated: check for that ?
- in Predict mode with functional data, the logStorage_ (defined in mixt_ConfIntParamStat.h) isn't initialized in setParamStorage() (defined in mixt_ConfIntParamStat.h), unlike statStorage_ .. But exportDataParam() (defined in mixt_FunctionalMixture.h) tries to get it nonetheless : what's the role of logStorage_ in Predict Mode ? should it be initialized or should exportDataParam() be modified ?

## Rank

- The identifiability of the model is not enforced. Opposite ranks with symmetric values (around 0.5) of dispersion would yield the same model.

## Ordinal model

- clarify if can be used with EdgeAuthorized or not. It seems it cannot. Test with generated data.
- apparently there are sometimes errors in the unit tests. Check those as they could explain other errors.

# Long Term

- launch N identical runs, compare the log observed likelihood and keep the better one
	- how to keep all the parameters and of the last best run ?
	- note that only the SEM phase has to be run n times, not the Gibbs sampling
	- each mixture has to be responsible for pushing / pulling the best parameters from a cache
	- add IMixture::pushParametersToCache and pullParametersFromCache methods
- this way, the data / param export at the end of the Gibbs is not affected