# Abstract

One of MixtComp strengths is the ability to easily add a new model. Since MixtComp uses the SEM algorithm, the API is rich, as functions are provided for initialization, validation of data. Some functions are useful for every models, while others are only here to provide funtionnality in specific cases. The most likely cases being a Markov chain internal to the model, but which needs to be initialized. The page [Algorithm Description](./MixtComp/docs/algoDesc.md) provides further information on where the various methods are called.

# Basic Concepts

There is an `IMixture` interface class that describe all models. It is located in the `src/lib/Mixture/mixt_IMixture.h` file. Some methods are implemented, but most are virtual and must be implement in a derived class. This architectures allows for runtime polymorphism in the `MixtureComposer` class. Here, all the instanciated models are contained in `std::vector<IMixture*> v_mixtures_;`. It is then possible to apply the same process to all variables, independently of the actual implementation, as in:

```
std::string MixtureComposer::checkSampleCondition(
		const Vector<std::set<Index>>& classInd) const {
	std::string warnLog = checkNbIndPerClass(classInd);
	for (ConstMixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end();
			++it) {
		warnLog += (*it)->checkSampleCondition(classInd);
	}

	return warnLog;
}
```

which calls `checkSampleCondition(classInd)` for each variable, and concatenate the resulting string to `warnLog`.

The parameters must be stored in the `IMixture` child. No other object will provide this information. In the similar way, the data used by the model is not stored anywhere else. Hence a model must track both parameters and data.

To simplify notations, `DerivedModel` will describe a model that is derived from `IMixture` and implements all the requested virtual methods.

# Common methods

*Those methods must be implemented for every model in a non trivial way.*

## std::string checkSampleCondition(const Vector<std::set<Index>>& classInd) const

This is a method called to check that the data sampled in `sampleUnobservedAndLatent` is consistent with MixtComp general requirements that no estimated continuous quantity could be on the border of the parameter space.

For example, `Gaussian::checkSampleCondition` checks that there are at least two individuals per class, otherwise it would be impossible to estimate the standard deviation.

Note that `IMixture::mStep` also check the data. The difference is that `IMixture::checkSampleCondition` checks that it is possible to estimate at all, while `IMixture::mStep` checks the validity of the resulting estimation. In the case of the Gaussian model, `Gaussian::checkSampleCondition` checks the number of observations per class, while `IMixture::mStep` checks that the standard deviation is not too close to 0 (which would mean that the estimated model is degenerate). This separation is historical, because at one point `IMixture::mStep` was never supposed to fail. `IMixture::checkSampleCondition` and `IMixture::mStep` could be merged to simplify the API.

`checkSampleCondition` should return an empty string if no problems have been detected, or a string containing a detailed description of the problem so that the user can check his data, change his model, or take any course of action that could remove the problem.

## std::string mStep(const Vector<std::set<Index>>& classInd)

Performs the estimation of the parameters. The MixtComp algorithm guarantees that when `mStep` is called, all latent variables have already been initialized properly by `initializeMarkovChain` and `sampleUnobservedAndLatent`. Hence `mStep` can work on completed data. The maximum likelihood estimator of the parameters should provide the new parameter values.

`mStep` should return an empty string if no problems have been detected, or a string containing a detailed description of the problem so that the user can check his data, change his model, or take any course of action that could remove the problem.

## void storeSEMRun(Index iteration, Index iterationMax)

During the SEM part of the algorithm, after the burnin, the estimated parameters must be stored at each iteration. The parameters estimated at the end of the SEM run are then deduced from the distribution of parameters obtained during the SEM run. The most common means of estimating are by mode ou median value. This behaviour must be implemented in `DerivedModel`.

When `storeSEMRun` is called with `iteration < iterationMax`, the current parameters must be stored.

When `storeSEMRun` is called with `iteration == iterationMax`, the current parameters must be stored. The final parameters must be estimated using the mode / median, and must overwrite the last parameters estimated using `mStep`.

## void storeGibbsRun(Index i, Index iteration, Index iterationMax)

During the Gibbs part of the algorithm, after the burnin, the completed latent variables must be stored at each iteration. The distribution of each latent variable in `DerivedModel`. The most common means of estimating are by mode ou median value. This behaviour must be implemented in `DerivedModel`.

When `storeSEMRun` is called with `iteration < iterationMax`, the current parameters must be stored.

When `storeSEMRun` is called with `iteration == iterationMax`, the current parameters must be stored. The final parameters must be estimated using the mode / median, and must overwrite the last parameters estimated using `mStep`.

## Real lnCompletedProbability(Index i, Index k) const
## Real lnObservedProbability(Index ind, Index k) const
## Index nbFreeParameter()
## void writeParameters()
## std::string setDataParam(RunMode mode)
## void exportDataParam() const
## void initData(Index ind)
## void initParam()
## void computeObservedProba()
## void initializeMarkovChain(Index i, Index k)
## bool sampleApproximationOfObservedProba()

## ~IMixture()

Of course, if your custom model allocates data on the stack, do not forget to free everything that is needed in the virtual destructor.

# Specialized methods

*Those methodes only need to be implemented if the model has some special requirements.*

## void sampleUnobservedAndLatent(Index ind, Index k)

The unobserved data refers to missing or partially observed data. This is data that has been provided by the user, even if the information was as scarce as simply aknowledging that nothing was available. There is a specific output for those variables, as the user usually wants more information, for example information on their distribution under the estimated distribution, or imputed values.

Latent data in the current context means data that the user is not supposed to interact with. Information on internal latent variables is not supposed to be conveyed to the user at the end of the run.

The SEM algorithm requires sampling of any unobserved / latent variable (an EM algorithm would integrate over those variables instead). The sampling should always be done using the distribution derived from the current parameter of the model. This method is ALWAYS called after parameters have been estimated one way or another.

This method has been implemented for every models currently present in MixtComp. But, it is not considered a "Common Method" because one could implement a model with no latent variables. For example a model in which every computation has a closed-form expression, and no missing data is managed.
