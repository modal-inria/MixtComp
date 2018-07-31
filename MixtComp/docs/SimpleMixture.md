All simple models share the same architecture. At the core resides the `SimpleMixture` template class located at [SimpleMixture.h](MixtComp/src/lib/Mixture/Simple/SimpleMixture.h).

Its declaration is:

```
template<typename Model, typename DataHandler, typename DataExtractor,
		typename ParamSetter, typename ParamExtractor>
class SimpleMixture: public IMixture
```

The template arguments DataHandler, DataExtractor, ParamSetter and ParamExtractor are all common to all mixture models used in MixtComp, and are used to perform IO operations. What's more peculiar is the `Model` template argument. It points to a class that defines everything needed for a simple model. So let's take the Gaussian model as an example, and tear it down. The entry point is [Gaussian.h](MixtComp/src/lib/Mixture/Simple/Gaussian/Gaussian.h).

The last lines of `SimpleMixture` contains declarations such as: `typename Model::Sampler sampler_;` or `typename Model::DataStat dataStat_;`. Those declarations indicate that `Model` is expected to provide the required typedefs. This is the case, as the first lines in the declaration of `Gaussian` contain:

```
typedef Vector<Real> Data;
typedef ConfIntDataStat<Real> DataStat;
typedef GaussianSampler Sampler;
typedef GaussianLikelihood Likelihood;
```

From that, we can deduce that `Model::Sampler` corresponds to `GaussianSampler`, for the Gaussian model. In turn, `GaussianSampler` is declared in [GaussianSampler.h](MixtComp/src/lib/Mixture/Simple/Gaussian/GaussianSampler.h). It will provide everything `SimpleMixture` expects from a `Model::Sampler`. Which means a `samplingStepNoCheck` method. It is called in the `SimpleMixture::sampleUnobservedAndLatent` method:

```
void sampleUnobservedAndLatent(Index ind, Index k) {
    sampler_.samplingStepNoCheck(ind, k);
}
```

The meaning of the various helper classes are:
- `Data` provides the data container. Some data is stored as integers, other as floats, etc... This must be declared here.
- `DataStat` provides statistical computation on missing data. It tracks sampled value and at the end computes statistics and imputed values.
- `Sampler` samples the missing data.
- `Likelihood` provides computation of observed and completed likelihood.
