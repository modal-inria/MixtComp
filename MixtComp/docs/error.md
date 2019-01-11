# Common errors and solutions


### Model

**Error:**
```
The model a has been selected to describe the variable varName but it is not implemented yet. Please choose an available model for this variable.
```

**Solution: ** Check the model names given in the *model* parameter. Available models are lists in [Data format](./dataFormat.md).


### Data format

**Error:**
```
in varName, individual i: 0 present an error. a is not recognized as a valid format.
```

**Solution: **
Bad data format. Please refer to [Data format](./dataFormat.md).



### Initialization problem

**Error:**
```
MixtureComposer::initializeLatent, problem with observation i, impossible to initialize latent values. Individual is too different from rest of population and has too many latent variables (for example, missing values).
```

**Solution: **
It can happen in the case of a *Multinomial* model with a rare modality (modality with a low frequency comparing to the number of individuals). Increasing *nInitPerClass* or refactorizing modalities can solve this problem .



### Empty class

**Error:**
```
MixtureComposer::checkNbIndPerClass: at least one class is empty.
Maybe you asked for more classes than the number of observations you provided. Or the constraints on the classes of each observations in a (semi) supervised case are too tight.
```

**Solution: **
One class becomes empty during the algorithm. 
The number of classes *nClass* is perhaps too high, decrease it can solve the problem. 
In a (semi-)supervised clustering case, the number of classes *nClass* is perhaprs too high regarding the values given for *z_class*. 



### Gaussian model

**Error:**
```
checkSampleCondition, error in variable varName
Gaussian variables must have at least two individuals per class. This is not the case for at least one class. You can check whether you have enough individuals regarding the number of classes.
```

**Solution: **
A gaussian variable requires two elements for initialization. If this message occurs, the number of classes *nClass* is too large (need to decrease *nClass*) or there is not enough inviduals for initialization (need to increase *nInitPerClass*).


**Error:**
```
mStep error in variable: varName
Gaussian variables must have a minimum standard deviation of 1.e-8 in each class. It is not the case in class: i. If some values are repeated often in this variable, maybe a Multinomial or a Poisson variable will describe it better.
```

**Solution: ** One class has a zero standard deviation because all elements in it are equal. The *Gaussian* model is perhaps not appropriate.
In the case of a "spike and slab" variable, if the spike is of value *x*, you can use an interval *[x-eps:x+eps]* instead of *x*. 



### Functional model

**Error:**
```
Error(s) in variable: varName with Func_CS model. The errors in the various classes are:
 Class: i: Not enough different values for t. Data points should be separated by at least 1.e-8
```

**Solution: **
This error occurs for *Func_CS* and *Func_SharedAlpha_CS* models. The number of subregressions *nSub* given in hyperparameters is too high, you need to decrease it.



### NegativeBinomial model

**Errors:**
```
NegativeBinomial model, parameter n divergence in class: i. 
```
```
NegativeBinomial variables must have a p value different from 0 or 1 in each class. It is not the case in class: i.
```

**Solution: ** 
The parameter estimation did not converge. You can retry with a different initialization, a different number of class (*nClass*) or with a *Poisson* model.
