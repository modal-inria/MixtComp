# Parallelism

The parallelism has been implemented in the most simple way possible, without interfering with the expression of the algorithms. This means that when adding a new model, the programmer should not consider the parallel execution. In fact, the programmer must code everything to run on a single core. The parallelism has been implemented at the level of the `MixtureComposer` class. Two different cases are considered.

## Data parallelism

This is the most common occurrence of parallelism in MixtComp. The hypothesis used here is that all observations are independent and identically distributed. Therefor, all probabilities can be computed at the same time, and sampling can also be performed in parallel. Have a look at:

```cpp
void MixtureComposer::sampleUnobservedAndLatent() {
#pragma omp parallel for
    for (Index i = 0; i < nInd_; ++i) {
        sampleUnobservedAndLatent(i);
    }
}
```

Since the observations are independent, sampling is done in parallel in `sampleUnobservedAndLatent`. All the code in `sampleUnobservedAndLatent` must be single threaded. This includes for example `IMixture::sampleUnobservedAndLatent` which must be single threaded too.

## Variable parallelism

This type of parallelism uses the fact that the maximum likelihood estimator of each variable can be computed independently. This is a consequence of the conditional independence hypothesis. Therefor, when performing an mStep, it is possible to do the computation variable by variable. Estimators are not coupled. Hence the `parallel for` pragma in `MixtureComposer`:

```cpp
#pragma omp parallel for // note that this is the only case where parallelism is not performed over observations, but over individuals
    for (Index v = 0; v < nVar_; ++v) {
        std::string currLog;
        currLog = v_mixtures_[v]->mStep(classInd); // call mStep on each variable
        if (0 < currLog.size()) {
            vecWarnLog[v] = "mStep error in variable: "
                    + v_mixtures_[v]->idName() + eol + currLog + eol;
        }
    }
```

Note that the various "warnLog" are aggregated in a `vecWarnLog` vector, to avoid race conditions when retrieving the results of `v_mixtures_[v]->mStep(classInd)`.
