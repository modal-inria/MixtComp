# Add a model in the different R packages

Once you have add a model in C++ (check [Statistical model description](./howToAddModel.md)), R packages need to be updated to use it.


## Documentation to update

First of all, add the model in the following documentation:

* [gitlab README](../../README.md)
* [Data format](./dataFormat.md)
* [Output object](./objectOutput.md)

## RMixtComp

Update the documentation in the [mixtCompCluster function](../RMixtComp/R/RMixtComp-package.R) and [getData function](../RMixtComp/R/MIXTCOMP_getData.R).

Add the new model in the [getParam function](../RMixtComp/R/MIXTCOMP_getter.R). The function returns a matrix where rows corresponds to classes and columns to the estimated parameters.

## RJsonMixtComp

Add the new model in the [convertParam function](../RJsonMixtComp/R/convertJsonRobject.R). The function changes the param output from JsonMixtComp to be the same as the RMixtComp output.

## RMixtCompPlot

Update [plotDataCI function](../RMixtCompPlot/R/MIXTCOMP_plotUnivariateDistributions.R) with the new model. It requires to add an [extractCI** function](../RMixtCompPlot/R/MIXTCOMP_extractCIbounds.R) that compute the mean and the confidence interval per class.

Update [plotParamConvergence function](../RMixtCompPlot/R/MIXTCOMP_plotConvergence.R) with the new model. It plots the evolution of the estimated parameters during the algorithm's run.

Update [plotDataBoxplot function](../RMixtCompPlot/R/MIXTCOMP_plotUnivariateBoxplots.R) with the new model. It plots a boxplot of the data per class.
