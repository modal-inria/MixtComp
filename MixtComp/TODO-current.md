# SemStrategy

- should be capable of returning a boolean indicating that at least 1 run has been completed successfully
	- a string is already returned, that should be enough
	- the string might contain the result of the last run, if all were unsuccessful
- initSEMCheck has to be removed and only initSEMNoCheck and runSEMNoCheck must be kept
- launch N identical runs, compare the log observed likelihood and keep the better one
	- how to keep all the parameters and of the last best run ?
	- note that only the SEM phase has to be run n times, not the Gibbs sampling
	- each mixture has to be responsible for pushing / pulling the best parameters from a cache
- this way, the data / param export at the end of the Gibbs is not affected
- if SEMAlgo only contains a single function, that function should be part of the Strategy, no need for an addditional object

# SemAlgo

- only keep runNoCheck

# Initialization

- What to do when an individual is not suited for initialization ?
	- for example when a functional does not have enough points to estimate all the subregressions ?
	- those are extreme cases that at present would result in a crash
	- initParam should return a string explaining any problem it encountered

# Unbounded likelihood

- even if unbounded likelihood is the only reason to abort a run, the emptying of a class should remain a reason to abort a run. It impacts every variable.

# IMixture

- remove samplingStepCheck
- samplingStepNoCheck becomes samplingStep
- keep checkSampleCondition, except that in the implementation, only return false for unbounded likelihood
	- the test must occur after samplingStep, to check that the completion is valid
	- this is not a priority
- add pushParametersToCache and pullParametersFromCache methods
- composer.p_zi() and composer.classInd() are there in all mixture, why not include them in IMixture ?

# removal of Gibbs

- Ordinal: remove zCondition parameters first, then modify until compilation
- All samplingStepCheck code can be removed from the samplers as it will never be called again

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
	- all those checks must be made in checkSampleCondition
- dans l'algo, pas de bornes, rajouter les bornes avant l'export de paramètres
- si en prédiction, avec bornes partout, la variable devient muette
- remove the Gibbs
- export the number of degenerate run, so that an expert could reject if there are too many