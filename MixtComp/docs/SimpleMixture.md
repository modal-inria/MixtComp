# Simple Mixture

A simple model is a model which has a simple internal structure and which use is widespread. For example, the Gaussian model is a simple model. All the simple models in MixtComp share a similar structure. Their data is stored in an [AugmentedData](MixtComp/src/lib/Data/AugmentedData.h) instance, and the various computation are distributed among similar helper classes.

A counter-example is the [Rank](MixtComp/src/lib/Mixture/Rank/RankMixture.h) model. It is much more complicated than a Gaussian model, and could not even use an `AugmentedData` instance to represent its data.

So, as a rule a thumb, if your model is a canonical model, teached in undergrate courses, try to implement it as a Simple Mixture. Otherwise, entirely derive the model from IMixture.

All simple models share the same architecture. At the core resides the `SimpleMixture` template class located at [SimpleMixture.h](MixtComp/src/lib/Mixture/Simple/SimpleMixture.h).

## General description

Its signature is:

```cpp
template<typename Graph, typename Model>
class SimpleMixture: public IMixture
```

The template arguments `Graph` is common to all mixture models used in MixtComp, and is used to perform IO operations (see [How to add a model](howToAddModel.md) for a description). What's more peculiar to `SimpleMixture` is its `Model` template argument. It points to a class that defines everything needed for a simple model. So let's take the Gaussian model as an example, and get into the details. The entry point is [Gaussian.h](MixtComp/src/lib/Mixture/Simple/Gaussian/Gaussian.h).

The last lines of `SimpleMixture` contains declarations such as: `typename Model::Sampler sampler_;` or `typename Model::DataStat dataStat_;`. Those declarations indicate that `Model` is expected to provide the required typedefs. This is the case, as the first lines in the declaration of `Gaussian` (which can be used as a `Model` template argument) contains:

```cpp
typedef Vector<Real> Data;
typedef ConfIntDataStat<Real> DataStat;
typedef GaussianSampler Sampler;
typedef GaussianLikelihood Likelihood;
```

From that, we can deduce that `Model::Sampler` corresponds to `GaussianSampler`, for the Gaussian model. In turn, `GaussianSampler` is declared in [GaussianSampler.h](MixtComp/src/lib/Mixture/Simple/Gaussian/GaussianSampler.h). It will provide everything `SimpleMixture` expects from a `Model::Sampler`. Which means a `samplingStepNoCheck` method. `samplingStepNoCheck` is called from the `SimpleMixture::sampleUnobservedAndLatent` method:

```cpp
void sampleUnobservedAndLatent(Index ind, Index k) {
    sampler_.samplingStepNoCheck(ind, k);
}
```

The meaning of the various template arguments are:

- `Data` provides the data container. Some data is stored as integers, other as floats, etc... This must be declared here.
- `DataStat` provides statistical computation on missing data. It tracks sampled value and at the end computes statistics and imputed values.
- `Sampler` samples the missing data.
- `Likelihood` provides computation of observed and completed likelihood.

## Statistics objects

Note that those classes can in turn rely on more low level statistical computation classes. Take `GaussianSampler` for example, it contains a `NormalStatistic normal_`. This object instance provides sampling of a normal distribution. It is used for example in the `GaussianSampler::samplingStepNoCheck` method, to sample a completely missing gaussian value:

```cpp
case missing_: {
  z = normal_.sample(mean,
                     sd);
}
break;
```

Note that creating a `NormalStatistic` instance is not a `SimpleMixture` requirement, as all the code for sampling from a normal distribution could have been written in the `GaussianSampler` class. However, it is such a low level statistical tool that it is highly likely to be used in several models. This is indeed the case, as the [Functional](MixtComp/src/lib/Mixture/Functional) uses it too.

The `GaussianLikelihood` class also takes advantages of `NormalStatistic`. It also contains a member `NormalStatistic normal_;`. But the purpose is a bit different. Here it is used to perform computations of probabilities. As in:

```cpp
Real GaussianLikelihood::lnCompletedProbability(int i, int k) const {
  Real mean  = param_(2 * k    );
  Real sd    = param_(2 * k + 1);

  Real logProba = normal_.lpdf(augData_.data_(i),
                               mean,
                               sd);

  return logProba;
}
```

`NormalStatistic::lpdf` is called here to compute the probability of the `i`-th observation, when its class is `k`.

As a general rule of thumb if the model you are coding is very generic, and this is often the case for SimpleMixture models, try to isolate statistical computations in a Statistics object.

## Other considerations

`Model::acceptedType` is used in `SimpleMixture::setDataParam`:

```cpp
std::string tempLog = augData_.checkMissingType(model_.acceptedType()); // check if the missing data provided are compatible with the model
```

The `Model::acceptedType` method provides a vector containing the type of missing data supported. It is used by `AugmentedData::checkMissingType` which will loop over all observations in the variable and check against the accepted missing type. If one observation is in a type that is not authorized, for example `[3:+inf]` for a categorical variable, a string will be returned to the user. He will be asked to check the data.
