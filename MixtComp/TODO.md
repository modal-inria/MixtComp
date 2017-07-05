# Current

## Initialization
- the mStep just after initParam will be suppressed, as it would erase the result of initParam
	- what is need after initParam are eStep and sStep
	- it is the case in both SEMAlgo::runCheck and SEMAlgo::runNoCheck at the moment, so everything is fine
- Functional
	- How to initialize alpha ? Are there no problem to initialize it from a single individual ?
- What to do when an individual is not suited for initialization ?
	- for example when a functional does not have enough points to estimate all the subregressions ?
- The uniform initialization seems useless now that one observation is used to initialize the parameters
	- is it so that only that one observation shall be uniformly completed ?

## IMixture
- remove samplingStepCheck
- samplingStepNoCheck becomes samplingStep
- keep checkSampleCondition, except that in the implementation, only return false for unbounded likelihood
	- the test must occur after samplingStep, to check that the completion is valid
- add pushParametersToCache and pullParametersFromCache methods
- composer.p_zi() and composer.classInd() are there in all mixture, why not include them in IMixture ?

## removal of Gibbs

- Ordinal: remove zCondition parameters first, then modify until compilation

## SemStrategy
- initSEMCheck has to be removed and only initSEMNoCheck and runSEMNoCheck must be kept
- launch 10 identical runs, compare the log observed likelihood and keep the better one
	- how to keep all the parameters and of the last best run ?
	- note that only the SEM phase has to be run n times, not the Gibbs sampling
	- each mixture has to be responsible for pushing / pulling the best parameters from a cache
- this way, the data / param export at the end of the Gibbs is not affected

## SemAlgo
- only keep runNoCheck

## Order of implementation, each step should result in a working version of MixtComp
- new initialization by partitioning and ordering, while keeping everything else as of today. This way the new initialization will benefit MixtComp while further updates are implemented
- updated parameter normalization, as this can be used with parameterEdgeAuthorized = TRUE in current version
- multiple initialization, without detection of degeneracy, only keeping the best run
	- only strategy is no check with normalization
- detection of degeneracy
	- normalization
- removal of Gibbs and all unused code (samplingStepCheck, etc ...)

## Multi run

- only end the computation if the likelihood is unbound.
	- For example, not when a a proportion in a categorical model is 0.
	- end it if the standard deviation is 0
- dans l'algo, pas de bornes, rajouter les bornes avant l'export de paramètres
- si en prédiction, avec bornes partout, la variable devient muette
- remove the Gibbs
- export the number of degenerate run, so that an expert could reject if there are too many

# Short Term

## Precise modifications

- in Predict mode with functional data, the logStorage_ (defined in mixt_ConfIntParamStat.h) isn't initialized in setParamStorage() (defined in mixt_ConfIntParamStat.h), unlike statStorage_ .. But exportDataParam() (defined in mixt_FunctionalMixture.h) tries to get it nonetheless : what's the role of logStorage_ in Predict Mode ? should it be initialized or should exportDataParam() be modified ?

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

# Various

- Since the algorithm is stochastic, it is complicated to obtain reproducible results during unit testing and continuous integration.
	- If the seeds of all RNG are fixed to the same value, then there is not enough variability in the generated data for the test to pass. Should the data be generated normally ?
	- If rng are shared, then there will be race conditions in multi-threading runs
	- So the deterministic approach is put on standby. mixt_RNG.h contains the seed generation.
	- This also highlight the problem that the algorithm is dependent on the initialization of the RNG and this should be dealt with...

# Estimation problem

- keep the test for degeneracy
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
    Les statistiques des données complétées au format matriciel (liste de vecteurs en ce moment)
- Poisson/Ordinal :
    Les statistiques des données complétées au format matriciel (liste de vecteurs en ce moment)

# Model

- Functional Model
    - Missing data in support in
- Ordinal model
    - clarify if can be used with EdgeAuthorized or not. It seems it cannot. Test with generated data.
    - apparently there are sometimes errors in the unit tests. Check those as they could explain other errors.
- remove regex in data parsing. Will be faster but less tolerant to data format.