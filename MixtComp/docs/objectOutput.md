# Overview of output object
Overview of output object with variables named *categorical*, *gaussian*, *ordinal*, *rank*, *functional*, *poisson*, *nBinom* and *weibull* with model respectively *Categorical_pjk*, *Gaussian_sjk*, *Ordinal*, *Rank*, *Functional* (or *FunctionalSharedAlpha*), *Poisson_k*, *NegativeBinomial* and *Weibull*.
The output object is a list of list, organized as follows:

```
output
|_______ strategy __ nbBurnInIter
|                |__ nbIter
|                |__ nbGibbsBurnInIter
|                |__ nbGibbsIter
|                |__ nInitPerClass
|                |__ nSemTry
|
|_______ mixture __ BIC
|               |__ ICL
|               |__ lnCompletedLikelihood
|               |__ lnObservedLikelihood
|               |__ IDClass
|               |__ delta
|               |__ mode
|               |__ runTime
|               |__ nbSample
|               |__ nbInd
|               |__ nbFreeParameters
|               |__ nbCluster
|               |__ warnLog
|               |__ completedProbabilityLogBurnIn
|               |__ completedProbabilityLogRun
|               |__ lnProbaGivenClass
|
|_______ variable __ type __ z_class
                 |       |__ categorical
                 |       |__ gaussian
                 |       |__ ...
                 |         
                 |__ data __ z_class __ completed
                 |	     |	        |__ stat
                 |	     |__ categorical __ completed
                 |	     |	            |__ stat
                 |	     |__ ...	
                 |	     |                 
                 |	     |__ functional __ data
                 |	                   |__ time
                 |	
                 |__ param __ z_class __ pi	__ stat
                          |                |__ log
                          |                |__ paramStr
                          |__ categorical __ NumericalParam __ stat
                          |                                |__ log
                          |                                |__ paramStr
                          |__ gaussian __ NumericalParam __ stat
                          |                             |__ log
                          |                             |__ paramStr
                          |__ poisson __ NumericalParam __ stat
                          |                            |__ log
                          |                            |__ paramStr
                          |__ nBinom __ NumericalParam __ stat
                          |                           |__ log
                          |                           |__ paramStr
                          |__ weibull __ NumericalParam __ stat
                          |                            |__ log
                          |                            |__ paramStr
                          |__ ordinal __ muPi __ stat
                          |                  |__ log
                          |                  |__ paramStr
                          |__ functional __ alpha __ stat
                          |             |        |__ log
                          |             |        |__ paramStr
                          |             |__ beta __ stat
                          |             |       |__ log
                          |             |       |__paramStr
                          |             |__ sd __ stat
                          |                   |__ log
                          |                   |__ paramStr
                          |__ rank __ mu __ stat
                                  |     |__ log
                                  |     |__ paramStr
                                  |__ pi __ stat
                                        |__ log
                                        |__ paramStr
```

# Output

## strategy
A copy of mcStrategy parameter.

- **nbBurnInIter** Number of iterations of the burn-in part of the SEM algorithm.
- **nbIter** Number of iterations of the SEM algorithm.
- **nbGibbsBurnInIter** Number of iterations of the burn-in part of the Gibbs algorithm.
- **nbGibbsIter** Number of iterations of the Gibbs algorithm.
- **nInitPerClass** Number of individuals used to initialize each cluster.
- **nSemTry** Number of try of the algorithm for avoiding an error.

## mixture
- **BIC** value of BIC
- **ICL** value of ICL
- **nbSample** number of samples in the dataset
- **nbInd** number of samples in the dataset
- **nbCluster** number of class of the mixture
- **nbFreeParameters** number of free parameters of the mixture model
- **lnObservedLikelihood** observed loglikelihood
- **lnCompletedLikelihood** completed loglikelihood
- **mode** "predict" for mixtCompPredict or "learn" for mixtCompCluster
- **IDClass** entropy used to compute the discriminative power (see code of plotDiscrimVbles)
- **delta** entropy used to compute the similarities between variables (see code of heatmapVbles)
- **completedProbabilityLogBurnIn** evolution of the completed log-probability during the burn-in period (can be used to check the convergence and determine the ideal number of iteration)
- **completedProbabilityLogRun** evolution of the completed log-probability after the burn-in period (can be used to check the convergence and determine the ideal number of iteration)
- **warnLog** contains warnings. NULL if there is no warning.
- **runTime** execution time in seconds
- **lnProbaGivenClass** log-(probability of each sample for each class times the proportion): $`\log(\pi_k)+\log(P(X_i|z_i=k))`$

## variable

### type
Model used for each variable.

### data

- **z_class**

Two elements: *completed* and *stat*. *completed* contains the completed data. *stat* is a matrix with the same number of columns as the number of class.
For each sample, it contains the $`t_{ik}`$ (probability of $`x_i`$ to belong to class $`k`$) estimated with the imputed values during the Gibbs at the end of each iteration after the burn-in phase of the algorithm.

- **Gaussian_sjk**

Two elements: *completed* and *stat*. *completed* contains the completed data. *stat* contains statistics about completed data. 
It is a list where each element corresponds to a missing data and contains a vector of 4 elements: index of the missing data, median, 2.5% quantile, 97.5% quantile (if the confidenceLevel parameter is set to 0.95) of imputed values during the Gibbs at the end of each iteration after the burn-in phase of the algorithm.

- **Categorical_pjk**

Two elements: *completed* and *stat*. *completed* contains the completed data. *stat* contains statistics about completed data. 
It is a list where each element corresponds to a missing data, each of them is a list with the first element corresponding to the index of the missing data, the others are the imputed values, during the Gibbs at the end of each iteration after the burn-in phase of the algorithm, and their frequency.

- **Poisson_k**

Two elements: *completed* and *stat*. *completed* contains the completed data. *stat* contains statistics about completed data. 
It is a list where each element corresponds to a missing data and contains a vector of 4 elements: index of the missing data, median, 2.5% quantile, 97.5% quantile (if the confidenceLevel parameter is set to 0.95) of imputed values during the Gibbs at the end of each iteration after the burn-in phase of the algorithm.

- **NegativeBinomial**

Two elements: *completed* and *stat*. *completed* contains the completed data. *stat* contains statistics about completed data. 
It is a list where each element corresponds to a missing data and contains a vector of 4 elements: index of the missing data, median, 2.5% quantile, 97.5% quantile (if the confidenceLevel parameter is set to 0.95) of imputed values during the Gibbs at the end of each iteration after the burn-in phase of the algorithm.

- **Weibull**

Two elements: *completed* and *stat*. *completed* contains the completed data. *stat* contains statistics about completed data. 
It is a list where each element corresponds to a missing data and contains a vector of 4 elements: index of the missing data, median, 2.5% quantile, 97.5% quantile (if the confidenceLevel parameter is set to 0.95) of imputed values during the Gibbs at the end of each iteration after the burn-in phase of the algorithm.


- **Ordinal**

Two elements: *completed* and *stat*. *completed* contains the completed data. *stat* contains statistics about completed data. 
It is a list where each element corresponds to a missing data, each of them is a list with the first element corresponding to the index of the missing data, the others are the imputed values, during the Gibbs at the end of each iteration after the burn-in phase of the algorithm, and their frequency.

- **Rank**

Two elements: *completed* and *stat*. *completed* contains the completed data. *stat* contains statistics about completed data. 
It is a list where each element corresponds to a missing data, each of them is a list with the first element corresponding to the index of the missing data, the others are the imputed values, during the Gibbs at the end of each iteration after the burn-in phase of the algorithm, and their frequency.

- **Functional** and **FunctionalSharedAlpha** 

Two elements: *data* and *time*. *time* (resp. *data*) is a list containing the time (resp. value) vector of the functional for each samples.


### param

- **z_class**

A list (named *pi*) of 3 elements: *stat*, *log*, *paramStr*.

*log* is matrix containing the estimated proportion during the M step of each iteration of the algorithm after the burn-in phase. *stat* is a matrix containing the median (and quantiles corresponding to the confidenceLevel parameter) of the estimated proportion. The median proportions are the returned proportions. *paramStr* contains "".


- **Gaussian_sjk**

A list (named *NumericalParam*) of 3 elements: *stat*, *log*, *paramStr*.

*log* is matrix containing the estimated mean and sd for each class during the M step of each iteration of the algorithm after the burn-in phase. *stat* is a matrix containing the median (and quantiles corresponding to the confidenceLevel parameter) of the estimated parameters. The median parameters are the returned parameters. *paramStr* contains "".


- **Categorical_pjk**

A list (named *NumericalParam*) of 3 elements: *stat*, *log*, *paramStr*.

*log* is matrix containing the estimated probability for each modality and class during the M step of each iteration of the algorithm after the burn-in phase. *stat* is a matrix containing the median (and quantiles corresponding to the confidenceLevel parameter) of the estimated parameters. The median parameters are the returned parameters. *paramStr* contains "nModality: J" where J is the number of modalities.


- **Poisson_k**

A list (named *NumericalParam*) of 3 elements: *stat*, *log*, *paramStr*.

*log* is matrix containing the estimated lambda parameter for each class during the M step of each iteration of the algorithm after the burn-in phase. *stat* is a matrix containing the median (and quantiles corresponding to the confidenceLevel parameter) of the estimated parameters. The median parameters are the returned parameters. *paramStr* contains "".

- **NegativeBinomial**

A list (named *NumericalParam*) of 3 elements: *stat*, *log*, *paramStr*.

*log* is matrix containing the estimated n and p parameters for each class during the M step of each iteration of the algorithm after the burn-in phase. *stat* is a matrix containing the median (and quantiles corresponding to the confidenceLevel parameter) of the estimated parameters. The median parameters are the returned parameters. *paramStr* contains "".

- **Weibull**

A list (named *NumericalParam*) of 3 elements: *stat*, *log*, *paramStr*.

*log* is matrix containing the estimated k and lambda parameters for each class during the M step of each iteration of the algorithm after the burn-in phase. *stat* is a matrix containing the median (and quantiles corresponding to the confidenceLevel parameter) of the estimated parameters. The median parameters are the returned parameters. *paramStr* contains "".


- **Ordinal**

A list (named *muPi*) of 3 elements: *stat*, *log*, *paramStr*.

*log* is matrix containing the estimated mu and pi parameters for each class during the M step of each iteration of the algorithm after the burn-in phase. *stat* is a matrix containing the median (and quantiles corresponding to the confidenceLevel parameter) of the estimated parameters. The median parameters are the returned parameters. *paramStr* contains "nModality: J" where J is the number of modalities.

- **Rank**

Two lists (named *mu* and *pi*) of 3 elements: *stat*, *log*, *paramStr*.

For the *pi* list (resp. *mu* list), *log* is matrix containing the estimated *pi* (resp. *mu*) parameter for each class during the M step of each iteration of the algorithm after the burn-in phase. *stat* is a matrix containing the median (and quantiles corresponding to the confidenceLevel parameter) (resp. mode) of the estimated pi (resp. mu) parameters. The median (resp. mode) parameters are the returned parameters. *paramStr* contains "nModality: J" where J is the length of the rank (number of sorted objects) dor tu *mu* list and "" for the *pi* list.

- **Functional** and **FunctionalSharedAlpha** 

Three lists (named *alpha*, *beta* and *sd*) of 3 elements: *stat*, *log*, *paramStr*.

*log* is matrix containing the estimated k and lambda parameter for each coefficient (number at the end of the parameter name) of each subregression (s: in the parameter name) for each class during the M step of each iteration of the algorithm after the burn-in phase. *stat* is a matrix containing the median (and quantiles corresponding to the confidenceLevel parameter) of the estimated parameters. The median parameters are the returned parameters. *paramStr* contains "nSub: S, nCoeff: C" where S is the number of sub-regressions and C the number of coefficients of each regression.


