# Abstract

One of MixtComp strengths is the ability to add models. The `IMixture` interface is rich (16 methods), as functions are provided for every aspect of model management. Some methods are useful for every models, while others are only here to provide functionality in specific cases. The page [Algorithm Description](./MixtComp/docs/algoDesc.md) provides further information on where the various methods are called from.

The file [recap.ods](./MixtComp/docs/180308 - Initialization/recap.ods) provides a recap of the operations used by various methods in initialization.

The folder [mise à jour majeur](./MixtComp/docs/170616 - mise à jour majeure) provides in-depth description of the initialization process.

# Basic Concepts

There is an `IMixture` interface class that describe all models. It is located in [mixt_IMixture.h](MixtComp/src/lib/Mixture/mixt_IMixture.h) file. Some methods are implemented, but most are virtual and must be implement in a derived class. This architectures allows for runtime polymorphism in the `MixtureComposer` class. Here, all the instantiated models are contained in `std::vector<IMixture*> v_mixtures_;`. It is then possible to apply the same process to all variables by looping over `v_mixtures_`, independently of the derived class, as in:

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

which calls `checkSampleCondition(classInd)` for each variable, and concatenates the resulting string to `warnLog`.

To simplify notations, `DerivedModel` will describe from now on a model that is derived from `IMixture` and implements all the requested virtual methods.

While IMixture is not a template class, `DerivedModel` must be, because `MixtureManager` is templated by `<typename DataHandler, typename DataExtractor, typename ParamSetter, typename ParamExtractor>`, and will pass references to a `DataHandler`, `DataExtractor`, `ParamSetter`, `ParamExtractor` to any `IMixture`-derived model. Those references will be used to inject / extract data to / from `DerivedModel`. This templating is used to provide various wrappers to MixtComp. There is a collection of {`DataHandler`, `DataExtractor`, `ParamSetter`, `ParamExtractor`} for R, another for Json. Should MixtComp ever interface with Python for example, a new set of {`DataHandler`, `DataExtractor`, `ParamSetter`, `ParamExtractor`} classes will have to be written.

# Implicit behaviour

A lot of interactions with `DerivedModel` are carried out through the call to its methods. However, some behaviour is implicit and can not be deduced from the API.

The parameters for example must be stored in `DerivedModel`. No other object will provide this information. In a similar way, the data used by `DerivedModel` is not stored anywhere else. Hence `DerivedModel` must track both parameters and data.

The initialization of the data in `DerivedModel` is a bit counter intuitive. One would expect data to be provided through a method call. This is not the case, for historical reasons. Instead, the `DataHandler` which was passed as a parameter of `DerivedModel` will be used. The method `DataHandler::getData` is called with the data to be modified passed as a reference. `DataHandler::getData` modifies it in place.

# Common methods

Those methods must be implemented for every model in a non trivial way.

## std::string checkSampleCondition(const Vector\<std::set\<Index\>\>& classInd) const

This is a method called to check that the data sampled in `sampleUnobservedAndLatent` is consistent with MixtComp general requirements that no estimated continuous quantity could be on the border of the parameter space.

For example, `Gaussian::checkSampleCondition` checks that there are at least two individuals per class, otherwise it would be impossible to estimate the standard deviation.

Note that `IMixture::mStep` also check the data. The difference is that `IMixture::checkSampleCondition` checks that it is possible to estimate at all, while `IMixture::mStep` checks the validity of the resulting estimation. In the case of the Gaussian model, `Gaussian::checkSampleCondition` checks the number of observations per class, while `IMixture::mStep` checks that the standard deviation is not too close to 0 (which would mean that the estimated model is degenerate). This separation is historical, because at one point `IMixture::mStep` was never supposed to fail. `IMixture::checkSampleCondition` and `IMixture::mStep` could be merged to simplify the API.

`checkSampleCondition` should return an empty string if no problems have been detected, or a string containing a detailed description of the problem so that the user can check his data, change his model, or take any course of action that could remove the problem.

## std::string mStep(const Vector\<std::set\<Index\>\>& classInd)

Performs the estimation of the parameters. The MixtComp algorithm guarantees that when `mStep` is called, all latent variables have already been initialized properly by `initializeMarkovChain` and `sampleUnobservedAndLatent`. Hence `mStep` can work on completed data. The maximum likelihood estimator of the parameters should provide the new parameter values.

`mStep` should return an empty string if no problems have been detected, or a string containing a detailed description of the problem so that the user can check his data, change his model, or take any course of action that could remove the problem.

## void storeSEMRun(Index iteration, Index iterationMax)

During the SEM part of the algorithm, after the burnin, the estimated parameters must be stored at each iteration. The parameters estimated at the end of the SEM run are then deduced from the distribution of parameters obtained during the SEM run. The most common means of estimating are by mode ou median value. This behaviour must be implemented in `DerivedModel`.

When `storeSEMRun` is called with `iteration < iterationMax`, the current parameters must be stored.

When `storeSEMRun` is called with `iteration == iterationMax`, the current parameters must be stored. The final parameters must be estimated using the mode / median, and must overwrite the last parameters estimated using `mStep`.

## Real lnCompletedProbability(Index i, Index k) const

Return the log completed probability of the i-th observation, if it belongs to the k-th class. Since this is the completed probability, the data eventually sampled in `sampleUnobservedAndLatent` must be used.

## Real lnObservedProbability(Index ind, Index k) const

Return the log completed probability of the i-th observation, if it belongs to the k-th class. Since this is the observed probability, the data eventually sampled in `sampleUnobservedAndLatent` must never be used.

## Index nbFreeParameter() const

Return the number of free parameters in the model. Useful to compute BIC / ICL model selection criteria.

## void writeParameters() const

Print the current parameters values in `std::cout`. The formatting is not standardized, but a compact output is expected. This is mostly used to display estimated parameters at the end of a run. Usually for debugging purposes.

## std::string setDataParam(RunMode mode)

When `setDataParam` is called, data in `DerivedModel` must be set to the data provided by the user, using `DataHandler::getData`. Parameters must also be set, if the code is in prediction mode, using `ParamSetter::getParam`.

Note that if the data type is complicated (not integer nor float), the data sent to `DataHandler::getData` must be a `std::vector<String>`, and the parsing must occur directly in the model. Historical reasons.

## void exportDataParam() const

This method is similar to `setDataParam`, but for data and param exports. Here, the data / param are exported using `DataExtractor::exportVals` and `ParamExtractor::exportParam`. `exportVals` and `exportParam` have been overloaded for the most common types. If one wants to add a new type of data export or if parameters are more complex, new overloaded versions of `exportVals` and `exportParam` must be implemented. That was the case for the Rank model, because there is a notion of "central rank", which did not exist for other models.

## ~IMixture()

Of course, if your custom model allocates data on the stack, do not forget to free everything that is needed in the virtual destructor.

# Specialized methods

Those methods only need to be implemented in a non trivial way if the model has some special requirements.

## void sampleUnobservedAndLatent(Index ind, Index k)

The unobserved data refers to missing or partially observed data. This is data that has been provided by the user, even if the information was as scarce as simply acknowledging that nothing was available. There is a specific output for those variables, as the user usually wants more information, for example information on their distribution under the estimated distribution, or imputed values.

Latent data in the current context means data that the user is not supposed to interact with. Information on internal latent variables is not supposed to be conveyed to the user at the end of the run.

The SEM algorithm requires sampling of any unobserved / latent variable (an EM algorithm would integrate over those variables instead). The sampling should always be done using the distribution derived from the current parameter of the model. This method is ALWAYS called after parameters have been estimated one way or another.

This method has been implemented for every models currently present in MixtComp. But, it is not considered a "Common Method" because one could implement a model with no latent variables. For example a model in which every computation has a closed-form expression, and no missing data is managed.

## void storeGibbsRun(Index i, Index iteration, Index iterationMax)

During the Gibbs part of the algorithm, after the burnin, the completed latent variables must be stored at each iteration. The distribution of each latent variable in `DerivedModel` is then used to impute the data. The most common means of estimating are by mode or median value. This behaviour must be implemented in `DerivedModel`.

When `storeGibbsRun` is called with `iteration < iterationMax`, the current latent variable values must be stored.

When `storeGibbsRun` is called with `iteration == iterationMax`, the current atent variable values must be stored. The final parameters must be estimated using the mode / median, and must overwrite the latent values sampled using  `sampleUnobservedAndLatent`.

## void initData(Index ind)

Prepare the data for the computation, for example by performing a first completion using uniform distribution. The point here is to have completed data, and be able to perform an `mStep`. But, since no `mStep` has been performed before, no meaningful distribution can be used for completion. Hence the uniform sampling, for example using `AugmentedData::removeMissing`. A model with no latent variable does not need to perform anything during this step, because it is ready to perform an `mStep` directly from the observed data.

## void initParam()

Some models are able to use closed form expressions for the maximum likelihood estimator of the parameters. However, others use iterative solvers, or Markov Chains. For those, `mStep` expects the estimated parameters to have been initialized, which is the purpose of `initParam`.

## void computeObservedProba()

`lnObservedProbability` can be expensive to compute. Sometimes it is easier to first compute the distribution, and then the log-likelihood of the data. This way, if a particular value is repeated, its value can be pulled from a cache any time needed. Some models also compute the log observed probability using sampling.The result of the sampling is cached, and the distribution is numerically deduced from the frequencies of sampled value. Note that in that case, an observed value could have an observed probability of 0, which renders the ICL / BIC criteria impossible to compute.

## void initializeMarkovChain(Index i, Index k)

Initialize the Markov Chain for models that contain one.

The difference with `initData` is that `initializeMarkovChain` is called after parameters have been initialized. This allows for multiple sampling using the correct parameters. The Markov Chain should be closer to equilibrium after `initializeMarkovChain` has been called.

## bool sampleApproximationOfObservedProba()

As mentioned in `computeObservedProba`, the observed probability could be 0 even if the observed probability is not 0. `sampleApproximationOfObservedProba` was a proposed solution to differentiate models in which the observed distribution is computed by sampling, and those for which it is computed using closed forms expressions. The idea is that a 0 probability from sampling could not be trusted and a 0 probability from closed form could be trusted. This was not a satisfactory solution, and this method is not used at the moment. The problem is that, no matter the parameters, there can be no 0 probability observations in Rank model for example. And, if the observed probability is 0 for an observation in every classe, MixtComp execution stops. This behaviour is legitimate for example if a particular modality has never been observed in the learning sample for categorical models. It is not legitimate for rank variables.

# How to register a model

Once a model has been written as a derived class of `IMixture`, MixtComp must be capable of instantiating it when it is encountered as a variable. The key class in this is `MixtureManager`, located at [mixt_MixtureManager.h](MixtComp/src/lib/Manager/mixt_MixtureManager.h). `MixtureManager::createMixtures` loop over all variables. `MixtureManager::createMixture` instantiate each model as a `IMixture*` pointer, using the name of the model as provided in the descriptor file.

Hence a new model must be managed through a new entry in `MixtureManager::createMixture`.

# Specificities of Simple models

*The article [SimpleMixture](SimpleMixture.md) provides a more detailed description of SimpleMixture.*

There is a set of models with very common features:
- Gaussian
- Multinomial
- Poisson
- Weibull

They are all template instantiations of `template<typename Model, typename DataHandler, typename DataExtractor, typename ParamSetter, typename ParamExtractor> class SimpleMixture: public IMixture`. They share the common traits of using `AugmentedData` for storing their data. The parameters are stored in `Vector<Real> param_;`. The differences among them is concentrated in the template type argument `Model`. A `Model` member object is stored in every Simple mixture: `Model model_;`. For example, the call to `mStep` is deferred to `Model::mStep`:

```
std::string mStep(const Vector<std::set<Index> >& classInd) {return model_.mStep(classInd);
}
```

The simple models are then defined in different classes. For the Gaussian model, look in the file [mise à jour majeur](MixtComp/src/lib/Mixture/Gaussian.h).

The relevance of this historical architecture could be a subject of debate, but the large amount of factored code helps when adding a new model.
