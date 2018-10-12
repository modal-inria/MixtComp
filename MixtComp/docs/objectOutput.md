# Overview of output object
Overview of output object with variables named *categorical*, *gaussian*, *rank*, *functional*, *poisson*, *nBinom* and *weibull* with model respectively *Multinomal*, *Gaussian*, *Rank_ISR*, *Func_CS* (or *Func_SharedAlpha_CS*), *Poisson*, *NegativeBinomial* and *Weibull*.
In case of a successfull run, the output object is a list of list (in R and Json), organized as follows:

```
output
|_______ algo __ nbBurnInIter
|		          |_ nbIter
|		          |_ nbGibbsBurnInIter
|		          |_ nbGibbsIter
|		          |_ nInitPerClass
|		          |_ nSemTry
|		          |_ mode
|		          |_ nInd
|		          |_ confidenceLevel
|		          |_ nClass
|
|_______ mixture __ BIC
|		             |_ ICL
|		             |_ lnCompletedLikelihood
|		             |_ lnObservedLikelihood
|		             |_ IDClass
|		             |_ delta
|		             |_ runTime
|		             |_ nbFreeParameters
|		             |_ completedProbabilityLogBurnIn
|		             |_ completedProbabilityLogRun
|		             |_ lnProbaGivenClass
|
|_______ variable	__ type __ z_class
                  |       |_ categorical
                  |       |_ gaussian
                  |       |_ ...
                  |
                  |_ data	__ z_class __ completed
                  |	      |	         |_ stat
                  |	      |_ categorical __ completed
                  |	      |	             |_ stat
                  |	      |_ ...	
                  |	      |_ functional	__ data
                  |	                    |_ time
                  |	
                  |_ param __ z_class	__ stat
                          |           |_ log
                          |           |_ paramStr
                          |_ functional __ alpha __ stat
                          |	            |	       |_ log
                          |	            |_ beta	__ stat
                          |	            |	      |_ log
                          |	            |_ sd __ stat
                          |	            |	    |_ log
                          |	            |_ paramStr
                          |_ rank __ mu	__ stat
                          |       |     |_ log
                          |       |_ pi __ stat
                          |       |	    |_ log
                          |       |_ paramStr
                          |	
                          |_ gaussian	__ stat
                          |	          |_ log
                          |	          |_ paramStr
                          |_ poisson __ stat
                          |	         |_ log
                          |	         |_ paramStr
                          |_ ...
```

# Output

## warnLog
In case of an unsuccessfull run, the output object is a list containing an element **warnLog** with all the warnings returned by MixtComp. 

## algo
A copy of algo parameter.

- **nbBurnInIter** Number of iterations of the burn-in part of the SEM algorithm.
- **nbIter** Number of iterations of the SEM algorithm.
- **nbGibbsBurnInIter** Number of iterations of the burn-in part of the Gibbs algorithm.
- **nbGibbsIter** Number of iterations of the Gibbs algorithm.
- **nInitPerClass** Number of individuals used to initialize each cluster.
- **nSemTry** Number of try of the algorithm for avoiding an error.
- **nInd** number of samples in the dataset
- **nClass** number of class of the mixture
- **mode** "predict" for mixtCompPredict or "learn" for mixtCompLearn

## mixture
- **BIC** value of BIC
- **ICL** value of ICL
- **nbFreeParameters** number of free parameters of the mixture model
- **lnObservedLikelihood** observed loglikelihood
- **lnCompletedLikelihood** completed loglikelihood
- **IDClass** entropy used to compute the discriminative power (see code of plotDiscrimVbles)
- **delta** entropy used to compute the similarities between variables (see code of heatmapVbles)
- **completedProbabilityLogBurnIn** evolution of the completed log-probability during the burn-in period (can be used to check the convergence and determine the ideal number of iteration)
- **completedProbabilityLogRun** evolution of the completed log-probability after the burn-in period (can be used to check the convergence and determine the ideal number of iteration)
- **runTime** execution time in seconds
- **lnProbaGivenClass** log-(probability of each sample for each class times the proportion): $`\log(\pi_k)+\log(P(X_i|z_i=k))`$

## variable

### type
Model used for each variable (e.g. "Gaussian").

### data
Except for functional models and LatentClass, data contains, for each variable, two elements: *completed* and *stat*. *completed* contains the completed data and *stat* contains statistics about completed data. 
The format is detailled below according to the model.

- **LatentClass**

Two elements: *completed* and *stat*. *completed* contains the completed data. *stat* is a matrix with the same number of columns as the number of class.
For each sample, it contains the $`t_{ik}`$ (probability of $`x_i`$ to belong to class $`k`$) estimated with the imputed values during the Gibbs at the end of each iteration after the burn-in phase of the algorithm.

- **Gaussian/Poisson/NegativeBinomial/Weibull**

*stat* is a list where each element corresponds to a missing data and contains a vector of 4 elements: index of the missing data, median, 2.5% quantile, 97.5% quantile (if the confidenceLevel parameter is set to 0.95) of imputed values during the Gibbs at the end of each iteration after the burn-in phase of the algorithm.

- **Multinomial**

*stat*is a list where each element corresponds to a missing data, each of them is a list with the first element corresponding to the index of the missing data, the others are the imputed values, during the Gibbs at the end of each iteration after the burn-in phase of the algorithm, and their frequency.


- **Rank_ISR**

*stat* is a list where each element corresponds to a missing data, each of them is a list with the first element corresponding to the index of the missing data, the others are the imputed values, during the Gibbs at the end of each iteration after the burn-in phase of the algorithm, and their frequency.

- **Func_CS** and **Func_SharedAlpha_CS** 

Two elements: *data* and *time*. *time* (resp. *data*) is a list containing the time (resp. value) vector of the functional for each sample.


### param
For one variable, it contains a list with estimated parameters (*param*), log recorded during the SEM (*log*) and hyperparameters if any (*paramStr*).
The output format depends of the model but in most of the case, *stat* is a matrix with 3 columns containing the median values of estimated parameters and quantile ate the desired confidence level,
*log* is matrix containing the estimated proportion during the M step of each iteration of the algorithm after the burn-in phase and *paramStr* is a string.
For the meaning of the parameters, user can refer to the documentation [data format](dataFormat.md).


- **LatentClass**

A list of 3 elements: *stat*, *log*, *paramStr*.
*log* is matrix containing the estimated proportion during the M step of each iteration of the algorithm after the burn-in phase. *stat* is a matrix containing the median (and quantiles corresponding to the confidenceLevel parameter) of the estimated proportion. The median proportions are the returned proportions. *paramStr* contains "".

- **Gaussian**

The *stat* matrix has 2*nClass rows. For a class $`k`$, parameters are mean ($`\mu_k`$) and sd ($`\sigma_k`$). The distribution function is defined by:

- **Poisson**

The *stat* matrix has nClass rows. For a class $`k`$, the parameter is lambda ($`\lambda_k`$). The probability function is defined by:

- **NegativeBinomial**

The *stat* matrix has 2*nClass rows. For a class $`k`$, parameters are n ($`n_k`$) and p ($`p_k`$). The probability function is defined by:

- **Weibull**

The *stat* matrix has 2*nClass rows. For a class $`j`$, parameters are k/shape ($`k_j`$) and lambda/scale ($`\lambda_j`$). The distribution function is defined by:

- **Multinomial**

*paramStr* contains "nModality: J" where J is the number of modalities.

The *stat* matrix has J*nClass rows. For a class $`k`$, parameters are probabilities to belong to modality $J$.

- **Rank_ISR**

*paramStr* contains "nModality: J" where J is the length of the rank (number of sorted objects).

Two lists (named *mu* and *pi*) of 2 elements: *stat*, *log*.

For *pi*, *stat* is a matrix with nClass rows. For a class $`k`$, parameter is pi ($`pi_k`$).

For *mu*, *stat* is a list with nClass elements. For a class $`k`$, a list is returned with the mode of the parameter ($`\mu_k`$), and the frequency of the mode during the SEM algorithm after the burnin phase.


- **Func_CS** and **Func_SharedAlpha_CS** 

*paramStr* contains "nSub: S, nCoeff: C" where S is the number of sub-regressions and C the number of coefficients of each regression.

Three lists (named *alpha*, *beta* and *sd*) of 2 elements: *stat*, *log*.

For *alpha*, *stat* is a matrix with 2\*S\*nClass rows. For a class $`k`$ and a subregression $`s`$, parameters areestimated coefficients of a logistic regression controlling the transition between subregressions.

For *beta*, *stat* is a matrix with S\*C\*nClass rows. For a class $`k`$ and a subregression $`s`$, parameters are estimated coefficient of the regression.

For *sd*, *stat* is a matrix with S*nClass rows. For a class $`k`$ and a subregression $`s`$, the parameter is the standard deviation of the residuals of the regression. 

