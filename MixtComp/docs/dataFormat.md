# Data format used in MixtComp

MixtComp can be used via JMixtComp or RMixtComp.
JMixtComp requires 3 json files: algo, data and descriptor. 
RMixtComp requires 3 R objects: algo (a list), data (a data.frame or a matrix) and descriptor (a list).


## Data

The data file is a csv file (; separator), each column corresponds to a variable. The first row contains the name of each variable.
Others rows correspond to the different samples. See the section associated with each model for more details about data format.

JSON:
```
{
    "varName1": ["elem1", "elem2", "elem3", "elem4"],
    "varName2": ["elem1", "elem2", "elem3", "elem4"],
    "varName3": ["elem1", "elem2", "elem3", "elem4"]
}
```

R:

| var1 | var2 | var3 | var4 | var5 | 
| :---: | :---: | :---: | :---: | :---: | :---: |
| $`x_{1,1}`$ | $`x_{1,2}`$ | $`x_{1,3}`$ | $`x_{1,4}`$ | $`x_{1,5}`$ |
| $`x_{2,1}`$ | $`x_{2,2}`$ | $`x_{2,3}`$ | $`x_{2,4}`$ | $`x_{2,5}`$ |

## Descriptor

Descriptor is a list describing the variables used for clustering and the model used. Each element corresponds to a variable and contains two elements: the model used (`type`), and the hyperparameters of the model if any (`paramStr`). When there is no hyperparameters, user must provide a void string "".

JSON:
```
{
    "varName1": {
        "type": "Model1",
        "paramStr": "Param1"
    },
    "varName2": {
        "type": "Model2",
        "paramStr": "Param2"
    },
    "varName3": {
        "type": "Model3",
        "paramStr": ""
    }
}
```

R:
```
list(varName1 = list(type = "Model1", paramStr = "param1"), 
     varName2 = list(type = "Model2", paramStr = "param2"),
     varName3 = list(type = "Model3", paramStr = ""))
```


## Models

### Overview

| Data type                    | Available models    | Restrictions  | Hyperparameters | 
| :--------------------------: | :-----------------: | :-----------: |:--------------: | 
| Real                         | Gaussian            |               |                 |
|                              | Weibull             | $`\geq 0`$    |                 |
| Integer                      | Poisson             | $`\geq 0`$    |                 |
|                              | NegativeBinomial    | $`\geq 0`$    |                 | 
| Categorical                  | Multinomial         |               | yes (but no need to provide it) |
| Rank                         | Rank_ISR            |               | yes (but no need to provide it) |
| Functional                   | Func_CS             |               | yes             |
|                              | Func_SharedAlpha_CS |               | yes             |


### Details

Eight models are available in (R)MixtComp: 

  - **Gaussian** for real data.  For a class $`k`$, parameters are the mean ($`\mu_k`$) and the standard deviation ($`\sigma_k`$). The distribution function is defined by:
  ```math
f_k(x) = \frac{1}{\sqrt{2\pi\sigma_k^2}}\exp{\left(-2\frac{(x-\mu_k)^2}{\sigma_k^2}\right)}
```
  - **Weibull** for positive real data (usually lifetime). For a class $`j`$, parameters are the shape ($`k_j`$) and the scale ($`\lambda_j`$). The distribution function is defined by:
  ```math
f_j(x) = \frac{k_j}{\lambda_j} \left(\frac{x}{\lambda_j}\right)^{k_j-1} \exp{\left(-\left(\frac{x}{\lambda_j}\right)^{k_j}\right)}
```
  - **Poisson** for positive integer data. For a class $`k`$, the parameter is the mean and variance ($`\lambda_k`$). The density mass function function is defined by:
  ```math
f_k(x) = \frac{\lambda^k}{k!}\exp{(-\lambda)}
```
  - **NegativeBinomial** for positive integer data. For a class $`k`$, parameters are the number of success ($`n_k`$) and the probability of success ($`p_k`$). The density mass function function is defined by:
  ```math
f_k(x) = \frac{\Gamma(x+n_k)}{x! \Gamma(n_k)} p_k^{n_k}(1-p_k)^x
```
  - **Multinomial** for categorical data. For a class $`k`$, the model has $`M`$ parameters $`p_{k,j},\, j=1,\dots,M`$, where $`M`$ the number of modalities, corresponding to the probabilities to belong to the modality $`j`$. 
  $`p_{k,j},\, j=1,\dots,M`$ must verify $`\sum_{j=1}^M p_{k,j} = 1`$. 
  The density mass function is defined by:
```math
f_k(x = j) = \prod_{j=1}^K p_{k,j}^{a_j} \quad \text{with} \quad a_j = \begin{cases}
   1 &\text{if } x = j \\
   0 &\text{otherwise} 
\end{cases}
```
  The hyperparameter $`M`$ does not require to be specified, it can be guess from the data. If tou want to specify it, add `"nModality: M"` in the appropriate field of the description object.
  
  - **Rank_ISR** for ranking data. For a class $`k`$, the two parameters are the central rank ($`\mu_k`$) and the probability of making a wrong comparison ($`\pi_k`$). See the [article](./article/rankModel) for more details. 
   Ranks have their size $`M`$ as hyperparameter. But it does not require to be specified, it can be guess from the data. If tou want to specify it, add `"nModality: M"` in the appropriate field of the description object.
  
  - **Func_CS** & **Func_SharedAlpha_CS** for functional data. Between individuals, functional data can have different length and different time values. The model segments every functional and clusters them. The segmentation is performed using polynomial regressions on subpart of functionals. The model requires to indicate the desired number of subregressions $`S`$, and the number of coefficients $`C`$ used in each subregression, this number corresponds to the polynomial's degree minus 1.
  These hyperparameters must be specified by `"nSub: S, nCoeff: C"` in the appropriate field of the descriptor object.
  
  For a class $`k`$ and a subregression $`s`$, parameters are $`\alpha_{k,s,0}`$ and $`\alpha_{k,s,1}`$ the estimated coefficients of a logistic regression controlling the transition between subregressions, $`\beta_{k,s,1},\dots,\beta_{k,s,C}`$ the estimated coefficient of the polynomial regression and $`\sigma_{k,s}`$ the standard deviation of the residuals of the regression.
  
   *Func_SharedAlpha_CS* is a variant of the *Func_CS* model with the alpha parameter shared between clusters. It means that the start and end of each subregression will be the same across the clusters.
   




## Data Format

### Real data: Gaussian & Weibull
Real values are saved with the dot as decimal separator.
Missing data are indicated by a $`?`$. Partial data can be provided through intervals denoted by $`[a:b]`$ where $`a`$ (resp. $`b`$) is a real or $`-inf`$ (resp. $`+inf`$).

| gaussian1 | gaussian2 |
| --- | --- | 
| -2.87 | [-2.1:1] | 
| ? |  [1.5:+inf] |
| [-inf:0] | 3.28  | 

### Real positive data: Weibull
Weibull data are real positive values with the dot as decimal separator.
Missing data are indicated by a $`?`$. Partial data can be provided through intervals denoted by
$`[a:b]`$ where $`a`$ and $`b`$ are positive reals ($`b`$ can be +inf).

| weib1 |
| --- | 
| 0.5 |  
| 12.5 | 
| ? |
| [2.5:+inf] |
| [0:8.2] |

### Counting data: Poisson & NegativeBinomial
Counting data are positive integer. Missing data are indicated by a $`?`$.

| count1 |
| --- | 
| 0 |  
| 5 | 
| ? |
| 3 |


### Categorical Data: Multinomial
Modalities must be consecutive integer with 1 as minimal value. Missing data are indicated by a $`?`$.
For partial data, a list of possible values can be provided by $`\{a_1,...,a_j\}`$, where $`a_i`$ denotes a modality.
 
Categorical data before formatting:

| categ1 | categ2 |
| --- | --- |
| married | large |
| single | small |
| status unknown | medium |
| divorced | small or medium |
| divorced or single | large |


after formatting:

| categ1 | categ2 |
| --- | --- |
| 2 | 3 | 
| 1 | 1 |
| ? | 2 |
| 3 | {1,2} |
| {1,3} | 3 |




### Rank data
The format of a rank is: $`o_1,..., o_j`$ where $`o_1`$ is an integer corresponding to the the number of the object ranked in 1st position.
For example: $`4,2,1,3`$ means that the fourth object is ranked first then the second object is in second position and so on.
Missing data can be specified by replacing and object by a $`?`$ or a list of potential object, for example: $`4, \{2~3\}, \{2~1\}, ?`$ means that
the object ranked in second position is either the object number 2 or the object number 3, then the object ranked in third position is either the object 2 or 1 and the last one can be anything.
A totally missing rank is spedified by $`?,?,...,?`$.

### Functional data: Func_CS & Func_SharedAlpha_CS
The format of a fonctional data is: $`time_1:value_1,..., time_j:value_j`$. Between individuals, functional data can have different length and different time values.
In the case of a functional model, `nSub: i, nCoeff: k` must be indicated in the third row of the descriptor file.
$`i`$ is the number of subregressions in a functional data and k the number of coefficients of each regression (2 = linear, 3 = quadratic, ...). Missing data are not supported.

For example if you have a time vector of `1 2 3 4 5 6 8 12` and a value vector of `0.8 0.6 0.88 0.42 0.62 0.75 0.72 0.66` for one individual, the MixtComp format is `1:0.8,2:0.6,3:0.88,4:0.42,5:0.62,6:0.75,8:0.72,12:0.66`

Descriptor file with functional variables:

| func1 | func2 |  
| --- | --- |  
| Functional | FunctionalSharedAlpha | 
|  nSub:4, nCoeff:2 | nSub:6, nCoeff:3 |  

### Missing data summary

|                                     | Multinomial     | Gaussian     | Poisson    | NegativeBinomial | Weibull      |     Rank_ISR             | Func_CS |  LatentClass  |
| ----------------------------------- | :-------------: | :----------: | :--------: | :--------------: | :----------: | :----------------------: | :-----: | :-----------: |
| Completely missing                  |  $`?`$          |  $`?`$       |  $`?`$     | $`?`$            | $`?`$        |  $`?,?,?,?`$             |         | $`?`$         | 
| Finite number of values authorized  |  $`\{a,b,c\}`$  |              |            |                  |              |  $`4,\{1~2\},3,\{1~2\}`$ |         | $`\{a,b,c\}`$ |
| Bounded interval                    |                 |  $`[a:b]`$   |            |                  | $`[a:b]`$    |                          |         |               |
| Right bounded interval              |                 | $`[-inf:b]`$ |            |                  | $`[0:b]`$    |                          |         |               | 
| Left bounded interval               |                 | $`[a:+inf]`$ |            |                  | $`[a:+inf]`$ |                          |         |               |


### (Semi-)Supervised clustering
To perform a (semi-)supervised clustering, user can add a variable named `z_class` in the data (with eventually some missing values) and descriptor files with LatentClass as model in the descriptor file.


## Example

data file
```
gauss1;categ1;pois1;negBin1;weib1;rank1;funct1;z_class
2.52235;1;5;3;3.78;1,2,3,4;1:3.5,2:2.8,3:4.8,5:6.2,6:3.2;?
0.81467;?;3;5;4.52;1,2,{3 4},{3 4};1:5.6,2:5.18,3:2.44,4:6.36,5:5.46;?
[1.25:2.3];2;6;?;[0:3.2];?,?,?,?;1:3.34,2:3.14,3:4.79,4:3.84,5:6.4,6:1.88,7:2.95,8:3.44,9:4.04,10:1.88
```

descriptor file
```
gauss1;categ1;pois1;negBin1;weib1;rank1;funct1;z_class
Gaussian_sjk;Categorical_pjk;Poisson_k;NegativeBinomial;Weibull;Ordinal;Rank;Functional;LatentClass
;;;;;;;nSub:2,nCoeff:2;
```