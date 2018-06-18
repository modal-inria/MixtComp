# Data format used in MixtComp

There is two ways of loading data: with a data csv file and a descriptor csv file or with two matrices containing the data as characters.

## Data file

The data file is a csv file (; separator), each column corresponds to a variable. The first row contains the name of each variable.
Others rows correspond to the different samples. See the section associated with each model for more details about data format.

| var1 | var2 | var3 | var4 | var5 | 
| --- | --- | --- | --- |--- |--- |
| $`x_{1,1}`$ | $`x_{1,2}`$ | $`x_{1,3}`$ | $`x_{1,4}`$ | $`x_{1,5}`$ |
| $`x_{2,1}`$ | $`x_{2,2}`$ | $`x_{2,3}`$ | $`x_{2,4}`$ | $`x_{2,5}`$ |

## Descriptor file

The descriptor file is a csv file (; separator) containing 3 rows. The first row contains the names of the variables used for clustering. It can contains less variables than the data file.
The second row contains the model associated with each variables (see below for available models). The third row is optional and contains hyperparameters for some models.
 
| var1 | var3 | var4 | 
| --- | --- | --- | 
| model1 | model1 | model3 | 
|  |  | param3 | 

 
## Models

Eight models are available in (R)MixtComp: 

  - **Gaussian_sjk** for real data. The model has two parameters the mean (*mu*) and the standard deviation (*sd*).
  - **Categorical_pjk** for quantitative data. The model has one parameter per modality (the probability of the modality).
  - **Poisson_k** for positive integer data. The model has one parameter (*lambda*) corresponding to mean and variance.
  - **Weibull** for positive real data (usually lifetime). The model has two parameters shape (*k*) and scale (*lambda*).
  - **Functional** for functional data. Between individuals, functional data can have different length and different time values. The model has 3 parameters: *alpha*, *beta* and *sd*. See the [article](https://chamroukhi.users.lmno.cnrs.fr/papers/Chamroukhi-PWRM-JournalClassif-2016.pdf) for mode details.
  - **FunctionalWithSharedAlpha** variant of the Functional model with the alpha parameter shared between clusters. It means that the start and end of each subregression will be the same across the clusters.
  - **Rank** for ranking data (sorted obejct). The model has two parameters, the central rank (*mu*) and the probability of making a wrong comparison (*pi*). See the [article](https://hal.inria.fr/hal-00743384) for more details.
  - **Ordinal** for quantitative data with an order between modalities (e.g. small < medium < large). The model has two parameters, the central modality (*mu*) and the probability of making a wrong comparison (*pi*). See the [article](https://hal.inria.fr/hal-01052447) for more details.

NB: the Ordinal model is DEPRECATED.
 
## Format

### Gaussian data
Gaussian data are real values with the dot as decimal separator.
Missing data are indicated by a $`?`$. Partial data can be provided through intervals denoted by $`[a:b]`$ where $`a`$ (resp. $`b`$) is a real or $`-inf`$ (resp. $`+inf`$).

| gaussian1 | gaussian2 |
| --- | --- | 
| -2.87 | [-2.1:1] | 
| ? |  [1.5:+inf] |
| [-inf:0] | 3.28  | 


### Categorical & ordinal Data
Modalities must be consecutive integer with 1 as minimal value. Missing data are indicated by a $`?`$.
For partial data, a list of possible values can be provided by $`\{a_1,...,a_j\}`$, where $`a_i`$ denotes a modality.
 
Categorical data before formatting:

| categ1 | ord1 |
| --- | --- |
| married | large |
| single | small |
| status unknown | medium |
| divorced | small or medium |
| divorced or single | large |


after formatting:

| categ1 | ord1 |
| --- | --- |
| 2 | 3 | 
| 1 | 1 |
| ? | 2 |
| 3 | {1,2} |
| {1,3} | 3 |

### Poisson Data
Poisson data must be positive integer. Missing data are indicated by a $`?`$.

| pois1 |
| --- | 
| 0 |  
| 5 | 
| ? |
| 3 |

### Weibull Data
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


### Rank data
The format of a rank is: $`o_1,..., o_j`$ where $`o_1`$ is an integer corresponding to the the number of the object ranked in 1st position.
For example: $`4,2,1,3`$ means that the fourth object is ranked first then the second object is in second position and so on.
Missing data can be specified by replacing and object by a $`?`$ or a list of potential object, for example: $`4, \{2~3\}, \{2~1\}, ?`$ means that
the object ranked in second position is either the object number 2 or the object number 3, then the object ranked in third position is either the object 2 or 1 and the last one can be anything.
A totally missing rank is spedified by $`?,?,...,?`$.

### Functional data
The format of a fonctional data is: $`time_1:value_1,..., time_j:value_j`$. Between individuals, functional data can have different length and different time values.
In the case of a functional model, `nSub: i, nCoeff: k` must be indicated in the third row of the descriptor file.
$`i`$ is the number of subregressions in a functional data and k the number of coefficients of each regression (2 = linear, 3 = quadratic, ...). Missing data are not supported.

For example if you have a time vector of `1 2 3 4 5 6 8 12` and a value vector of `0.8 0.6 0.88 0.42 0.62 0.75 0.72 0.66` for one individual, the MixtComp format is `1:0.8,2:0.6,3:0.88,4:0.42,5:0.62,6:0.75,8:0.72,12:0.66`

Descriptor file with functional variables:

| func1 | func2 |  
| --- | --- |  
| Functional | FunctionalWithSharedAlpha | 
|  nSub:4, nCoeff:2 | nSub:6, nCoeff:3 |  

### Missing data summary

|                                     | Categorical_pjk | Gaussian_sjk | Poisson_k  | Ordinal       | Weibull      |     Rank                 | Functional |  LatentClass  |
| ----------------------------------- | :-------------: | :----------: | :--------: | :-----------: | :----------: | :----------------------: | :--------: | :-----------: |
| Completely missing                  |  $`?`$          |  $`?`$       |  $`?`$     | $`?`$         | $`?`$        |  $`?,?,?,?`$             |            | $`?`$         | 
| Finite number of values authorized  |  $`\{a,b,c\}`$  |              |            | $`\{a,b,c\}`$ |              |  $`4,\{1~2\},3,\{1~2\}`$ |            | $`\{a,b,c\}`$ |
| Bounded interval                    |                 |  $`[a:b]`$   |            |               | $`[a:b]`$    |                          |            |               |
| Right bounded interval              |                 | $`[-inf:b]`$ |            |               | $`[0:b]`$    |                          |            |               | 
| Left bounded interval               |                 | $`[a:+inf]`$ |            |               | $`[a:+inf]`$ |                          |            |               |


### (Semi-)Supervised clustering
To perform a (semi-)supervised clustering, user can add a variable named `z_class` in the data (with eventually some missing values) and descriptor files with LatentClass as model in the descriptor file.


## Example

data file
```
gauss1;categ1;pois1;weib1;ord1;rank1;funct1;z_class
2.52235;1;5;3.78;{1,2};1,2,3,4;1:3.5,2:2.8,3:4.8,5:6.2,6:3.2;?
0.81467;?;3;4.52;3;1,2,{3 4},{3 4};1:5.6,2:5.18,3:2.44,4:6.36,5:5.46;?
[1.25:2.3];2;6;[0:3.2];2;?,?,?,?;1:3.34,2:3.14,3:4.79,4:3.84,5:6.4,6:1.88,7:2.95,8:3.44,9:4.04,10:1.88
```

descriptor file
```
gauss1;categ1;pois1;weib1;ord1;rank1;funct1;z_class
Gaussian_sjk;Categorical_pjk;Poisson_k;Weibull;Ordinal;Rank;Functional;LatentClass
;;;;;;nSub:2,nCoeff:2;
```