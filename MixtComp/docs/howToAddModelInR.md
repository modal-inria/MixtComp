# Add a model in the different R packages

Once you have add a model in C++ (check [Statistical model description](./howToAddModel.md)), some functions in different R packages need to be updated to use it.

## Documentation to update

First of all, add the model in the following documentation:

* [gitlab README](../../README.md)
* [Data format](./dataFormat.md)
* [Output object](./objectOutput.md)

## RMixtComp

Update the documentation in the [mixtCompLearn function](../RMixtComp/R/MIXTCOMP_mixtCompLearn.R).

Add the new model in the [getParam function](../RMixtCompUtilities/R/MIXTCOMP_getParam.R). The function returns a matrix where rows correspond to classes and columns to the estimated parameters.

Add the new model in the [availableModels function](../RMixtCompUtilities/R/MIXTCOMP_misc.R). The function returns a data.frame containing all the models and some information (missing data format, hyperparameters...).

If the model requires some hyperparameters and you want to provide default values, you can add them in the [completeModel function](../RMixtCompUtilities/R/MIXTCOMP_formatData.R).

Update [plotDataCI function](../RMixtCompUtilities/R/PLOT_plotUnivariateDistributions.R) with the new model. It requires to add an [extractCI** function](../RMixtCompUtilities/R/PLOT_extractCIbounds.R) that compute the theorical mean and the confidence interval per class.

Update [plotDataBoxplot function](../RMixtCompUtilities/R/PLOT_plotUnivariateBoxplots.R) with the new model. It plots a boxplot of the data per class.

Update [plotParamConvergence function](../RMixtCompUtilities/R/PLOT_plotConvergence.R) with the new model. It plots the evolution of the estimated parameters during the algorithm's run.
