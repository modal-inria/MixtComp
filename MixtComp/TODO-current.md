# Initialization

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

# IMixture

- remove samplingStepCheck
- samplingStepNoCheck becomes samplingStep
- keep checkSampleCondition, except that in the implementation, only return false for unbounded likelihood
	- the test must occur after samplingStep, to check that the completion is valid
- add pushParametersToCache and pullParametersFromCache methods
- composer.p_zi() and composer.classInd() are there in all mixture, why not include them in IMixture ?

# removal of Gibbs

- Ordinal: remove zCondition parameters first, then modify until compilation

# SemStrategy

- initSEMCheck has to be removed and only initSEMNoCheck and runSEMNoCheck must be kept
- launch 10 identical runs, compare the log observed likelihood and keep the better one
	- how to keep all the parameters and of the last best run ?
	- note that only the SEM phase has to be run n times, not the Gibbs sampling
	- each mixture has to be responsible for pushing / pulling the best parameters from a cache
- this way, the data / param export at the end of the Gibbs is not affected

# SemAlgo

- only keep runNoCheck

# Order of implementation, each step should result in a working version of MixtComp

- new initialization by partitioning and ordering, while keeping everything else as of today. This way the new initialization will benefit MixtComp while further updates are implemented
- detection of degeneracy, and clean stop of the run, so the user can launch it again
	- normalization
- updated parameter normalization, as this can be used with parameterEdgeAuthorized = TRUE in current version
- multiple initialization, without detection of degeneracy, only keeping the best run
	- only strategy is no check with normalization
- removal of Gibbs and all unused code (samplingStepCheck, etc ...)

# Multi run

- only end the computation if the likelihood is unbound.
	- For example, not when a a proportion in a categorical model is 0.
	- end it if the standard deviation is 0
- dans l'algo, pas de bornes, rajouter les bornes avant l'export de paramètres
- si en prédiction, avec bornes partout, la variable devient muette
- remove the Gibbs
- export the number of degenerate run, so that an expert could reject if there are too many