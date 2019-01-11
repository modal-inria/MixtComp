This file contains a summary description of the algorithms, and outline

The easiest way to understand how to program MixtComp might be to add a new model from scratch, and

The learning case is considered, because once it has been understood, the prediction algorithm becomes easy.

This is a general overview of the algorithm. If one method is called from another, it is shown with one additional level of indentation. Reading order is calling order.

The SemStrategy and GibbsStrategy are coded in the [Strategy](./MixtComp/src/lib/Strategy) folder. The core algorithm is contained here.

# R interface

See [mixtCompCluster.cpp](./RMixtComp/src/mixtCompCluster.cpp) for the entry point to the learning algorithm, and [mixtCompPredict.cpp](./RMixtComp/src/mixtCompPredict.cpp) for the entry point to the predict algorithm.

The learn alorithm used the SemStrategy, then the GibbsStrategy, while the predict algorithm just uses the GibbsStrategy.

# SemStrategy run

Source [here](./MixtComp/src/lib/Strategy/SEMStrategy.cpp).

The main difference between an SEM run and a Gibbs run is that estimations take place. Those estimation must be tracked, and they can also wrong. The SEMStrategy is similar, except that no estimation take place, and the parameters are fixed all along.

Things can go wrong at various points in the algorithm, for example there can be errors in initialization, or degeneracy in the general execution. That is why there is a global loop `for (Index n = 0; n < param_.nSemTry_; ++n)` that is run `param_.nSemTry_` times at maximum. If an iteration is completed, the loop is exited. If no iteration can be completed, the `std::string` error from the last iteration is returned. An attempt consist in all key steps for a SEM run;

- initData is called to initialize all latent variables uniformly (observed constraints are satisfied, for example {1, 3, 5} for categorical data).
- checkNbIndPerClass checks that there is at least one observation in each class (useless ?).
- initParam initialize the parameters for the next step
- initParamSubPartition initialize the parameters by generating a subpopulation, and performing an mStep on it. The (approximate) default number of observation per class is given by the user. A checkSampleCondition is performed. This step can fail, hence the test on the size of warnLog which might trigger "continue".
- writeParameters outputs the parameters, which is useful for debug, or to check if the numer of observations per class is correct for initialization.
- initializeLatent performs the initialization of all latent variables, using the parameters previously estimated.
    - computeObservedProba asks each variable to compute the observed distributions if needed.
    - setObservedProbaCache computes components of the observed probability for various components. The idea is that the log probabilities are used in various contexts, and a cache should be used to accelerate accesses.
    - eStepObserved computes $t_{ik}$ based on observed initData, if an observation can not be assigned to any class, an error message is returned
    - sampleZ assigns classes
    - sampleUnobservedAndLatent sample all latent values using the parameters
    - checkSampleCondition is performed, but it might not be useful, because the next step in the algorithm will be an eStep
- SEM is called for burnin, a loop `for (Index iter = 0; iter < nIter; ++iter)` is performed. Each step contains:
    - eStepCompleted, to compute the `t_{ik}` based on completed data
    - sampleZ, to sample the class of every individual
    - sampleUnobservedAndLatent to sample the latent variables
    - checkSampleCondition to check that an mStep could be performed from the completed data
    - mStep
    - storeSemRun: do nothing in the burning
- SEM is called for run
    - similar to SEM for burnin, except that storeSemRun will record the parameters at each iteration, and perform an estimation based on the mean / mode on the last iteration
- Note that the observed probability cache is not updated here, because it will be updated at the beginning of the GibbsStrategy.

# GibbsStrategy run

The GibbsStrategy is similar to the SemStrategy, albeit a bit simpler. In particular, there is no management of the parameters, which are fixed. storeGibbsRun is similar to storeSemRun, but keeps track of data evolution instead of parameters evolution.

- initData
- initializeLatent
- Gibbs for burnin
    - eStepCompleted
    - sampleZ
    - sampleUnobservedAndLatent
    - storeGibbsRun: do nothing in the burning
- Gibbs for run
    - similar to SEM for burnin, except that storeGibbsRun will record the missing variables and class labels at each iteration, and perform an imputation based on the mean / mode on the last iteration

The GibbsStrategy can go wrong, for example if an individual has a 0 probability in every class (there are `nSemTry_`). The difference with `SemStrategy` is that, once initialization has been performed, there can be no error, because parameters have already been estimated, therefor degeneracy is not a problem. This is why `GibbsStrategy::runGibbs` returns `void`, not a `std::string`.

There is a try / catch exception handling, which main purpose is to avoid crashes. In a normal run, it is impossible for $t_{ik}$ values to be `nan`. However, errors in implementations in the model can lead to this. To avoid this kind of problems, an exception can be thrown from `MixtureComposer::eStepCompleted`. The program will then end normally, urging the user to contact the maintainer.
