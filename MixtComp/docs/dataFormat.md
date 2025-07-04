# Data format used in MixtComp

MixtComp can be used via *JMixtComp* or *RMixtComp*.
JMixtComp requires 3 json files: algo, data and descriptor.
RMixtComp requires 3 R objects: algo (a list), data (a list, a data.frame or a matrix) and descriptor (a list).

## Data

Data must have one of the following format. Each variable must be named.

### JSON data

```json
{
    "varName1": ["elem11", "elem12", "elem13", "elem14"],
    "varName2": ["elem21", "elem22", "elem23", "elem24"],
    "varName3": ["elem31", "elem32", "elem33", "elem34"]
}
```

### R data

```r
data <- list(
  varName1 = c(elem11, elem12, elem13, elem14),
  varName2 = c(elem21, elem22, elem23, elem24),
  varName3 = c(elem31, elem32, elem33, elem34),
)

data <- data.frame(
  varName1 = c(elem11, elem12, elem13, elem14),
  varName2 = c(elem21, elem22, elem23, elem24),
  varName3 = c(elem31, elem32, elem33, elem34),
)

data <- matrix(c(elem11, elem12, elem13, elem14,
                 elem21, elem22, elem23, elem24,
                 elem31, elem32, elem33, elem34), ncol = 3, dimnames = list(NULL, c("varName1", "varName2", "varName3")))
```

## Descriptor (model)

Descriptor is a list describing the variables used for clustering and the model used. Each element corresponds to a variable and contains two elements: the model used (`type`), and the hyperparameters of the model if any (`paramStr`). When there is no hyperparameters, user must provide a void string `""`. The descriptor object can contain less variables than the data object. Only variables listed in the descriptor object are used for clustering.

### JSON descriptor

```json
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

### R descriptor

```r
desc <- list(varName1 = list(type = "Model1", paramStr = "param1"),
             varName2 = list(type = "Model2", paramStr = "param2"),
             varName3 = list(type = "Model3", paramStr = ""))
```

## Algo

Algo is a list containing the required parameters of the SEM algorithm.

- **nbBurnInIter** Number of iterations of the burn-in part of the SEM algorithm.
- **nbIter** Number of iterations of the SEM algorithm.
- **nbGibbsBurnInIter** Number of iterations of the burn-in part of the Gibbs algorithm.
- **nbGibbsIter** Number of iterations of the Gibbs algorithm.
- **nInitPerClass** Number of individuals used to initialize each cluster.
- **nSemTry** Number of try of the algorithm for avoiding an error.
- **confidenceLevel** Confidence level for confidence bounds for parameter estimation.
- **ratioStableCriterion** Stability partition required to stop earlier the SEM .
- **nStableCriterion** Number of iterations of partition stability to stop earlier the SEM.
- **nInd** Number of individuals per variables.
- **nClass** Number of classes.

User can add extra elements, they will be copied in the output object.

### JSON algorithm

```json
{
  "nClass": 2,
  "nInd": 15,
  "nbBurnInIter": 100,
  "nbIter": 100,
  "nbGibbsBurnInIter": 100,
  "nbGibbsIter": 100,
  "nInitPerClass": 2,
  "nSemTry": 10,
  "confidenceLevel": 0.95,
  "ratioStableCriterion": 0.9,
  "nStableCriterion": 7,
  "notes": "You can add any note you wish in non mandatory fields like this one (notes). They will be copied to the output."
}
```

### R algorithm

```r
algo <- list(nClass = 2,
             nInd = 15,
             nbBurnInIter = 100,
             nbIter = 100,
             nbGibbsBurnInIter = 100,
             nbGibbsIter = 100,
             nInitPerClass = 2,
             nSemTry = 10,
             confidenceLevel = 0.95,
             ratioStableCriterion = 0.9,
             nStableCriterion = 7,
             notes = "You can add any note you wish in non mandatory fields like this one (notes). They will be copied to the output.")
```

In the RMixtComp, *nInd* can be omitted and *nClass* is copied from from mixtCompLearn function's argument.

## Models

Eight models are available in (R)MixtComp

### Overview

| Available models    | Data type                    | Restrictions  | Hyperparameters |
| :-----------------: | :--------------------------: | :-----------: |:--------------: |
| Gaussian            | Real                         |               |                 |
| Weibull             | Real                         | `>= 0`    |                 |
| Poisson             | Integer                      | `>= 0`    |                 |
| NegativeBinomial    | Integer                      | `>= 0`    |                 |
| Multinomial         | Categorical                  |               | yes (but no need to provide it) |
| Rank_ISR            | Rank                         |               | yes (but no need to provide it) |
| Func_CS             | Functional                   |               | yes             |
| Func_SharedAlpha_CS | Functional                   |               | yes             |

### Details

#### Gaussian

For real data.  For a class *k*, parameters are the mean ($`\mu_k`$) and the standard deviation ($`\sigma_k`$). The distribution function is defined by:

```math
f_k(x) = \frac{1}{\sqrt{2\pi\sigma_k^2}}\exp{\left(-2\frac{(x-\mu_k)^2}{\sigma_k^2}\right)}
```

#### Weibull

For positive real data (usually lifetime). For a class *j*, parameters are the shape (`k_j`) and the scale (`\lambda_j`). The distribution function is defined by:

```math
f_j(x) = \frac{k_j}{\lambda_j} \left(\frac{x}{\lambda_j}\right)^{k_j-1} \exp{\left(-\left(\frac{x}{\lambda_j}\right)^{k_j}\right)}
```

#### Poisson

For positive integer data. For a class *k*, the parameter is the mean and variance (`\lambda_k`). The density mass function is defined by:

```math
f_k(x) = \frac{\lambda^k}{k!}\exp{(-\lambda)}
```

#### NegativeBinomial

For positive integer data. For a class *k*, parameters are the number of success (`n_k`) and the probability of success (`p_k`). The density mass function is defined by:

```math
f_k(x) = \frac{\Gamma(x+n_k)}{x! \Gamma(n_k)} p_k^{n_k}(1-p_k)^x
```

#### Multinomial

For categorical data. For a class *k*, the model has *M* parameters `p_{k,j},\, j=1,...,M`, where *M* is the number of modalities, corresponding to the probabilities to belong to the modality `j`. `p_{k,j},\, j=1,...,M` must verify `\sum_{j=1}^M p_{k,j} = 1`.

The density mass function is defined by:

```math
f_k(x = j) = \prod_{j=1}^K p_{k,j}^{a_j} \quad \text{with} \quad a_j = \begin{cases}
   1 &\text{if } x = j \\
   0 &\text{otherwise}
\end{cases}
```

The hyperparameter *M* does not require to be specified, it can be guess from the data. If tou want to specify it, add `"nModality: M"` in the appropriate field of the description object.

#### Rank_ISR

For ranking data. For a class *k*, the two parameters are the central rank (`\mu_k`) and the probability of making a wrong comparison (`\pi_k`). See the [article](https://hal.inria.fr/hal-00743384) for more details. Ranks have their size *M* as hyperparameter. But it does not require to be specified, it can be guess from the data. If tou want to specify it, add `"nModality: M"` in the appropriate field of the description object.

#### Func_CS and Func_SharedAlpha_CS

For functional data. Between individuals, functional data can have different length and different time values. The model segments every functional and clusters them. The segmentation is performed using polynomial regressions on subpart of functionals. The model requires to indicate the desired number of subregressions *S*, and the number of coefficients *C* used in each subregression, this number corresponds to the polynomial's degree minus 1.

These hyperparameters must be specified by `"nSub: S, nCoeff: C"` in the appropriate field of the descriptor object. See the [article](https://arxiv.org/abs/1312.6967) for more details.

For a class *k* and a subregression *s*, parameters are `\alpha_{k,s,0}` and `\alpha_{k,s,1}` the estimated coefficients of a logistic regression controlling the transition between subregressions, `\beta_{k,s,1},...,\beta_{k,s,C}` the estimated coefficient of the polynomial regression and `\sigma_{k,s}` the standard deviation of the residuals of the regression.

*Func_SharedAlpha_CS* is a variant of the *Func_CS* model with the alpha parameter shared between clusters. It means that the start and end of each subregression will be the same across the clusters.

## Data Format

### Real data: Gaussian

Real values are saved with the dot as decimal separator.
Missing data are indicated by a `?`. Partial data can be provided through intervals denoted by `[a:b]` where `a` (resp. `b`) is a real or `-inf` (resp. `+inf`).

#### JSON real data

```json
{
    "varGauss1": ["2.1", "-0.26", "?", "[0.56:1.28]", "1.21", "[-inf:-0.11]", "[-1.65:+inf]"]
}
```

#### R real data

```r
data <- list(
  varGauss1 = c("2.1", "-0.26", "?", "[0.56:1.28]", "1.21", "[-inf:-0.11]", "[-1.65:+inf]")
)

data <- data.frame(
  varGauss1 = c("2.1", "-0.26", "?", "[0.56:1.28]", "1.21", "[-inf:-0.11]", "[-1.65:+inf]")
)

data <- matrix(c("2.1", "-0.26", "?", "[0.56:1.28]", "1.21", "[-inf:-0.11]", "[-1.65:+inf]"), ncol = 1, dimnames = list(NULL, c("varGauss1")))
```

### Real positive data: Weibull

Weibull data are real positive values with the dot as decimal separator.
Missing data are indicated by a `?`. Partial data can be provided through intervals denoted by
`[a:b]` where `a` and `b` are positive reals (`b` can be `+inf`).

#### JSON real positive data

```json
{
    "varWeib1": ["2.1", "0.26", "?", "[0.56:1.28]", "1.21", "[0:5.11]", "[1.65:+inf]"]
}
```

#### R real positive data

```r
data <- list(
  varWeib1 = c("2.1", "0.26", "?", "[0.56:1.28]", "1.21", "[0:5.11]", "[1.65:+inf]")
)

data <- data.frame(
  varWeib1 = c("2.1", "0.26", "?", "[0.56:1.28]", "1.21", "[0:5.11]", "[1.65:+inf]")
)

data <- matrix(c("2.1", "0.26", "?", "[0.56:1.28]", "1.21", "[0:5.11]", "[1.65:+inf]"), ncol = 1, dimnames = list(NULL, c("varWeib1")))
```

### Counting data: Poisson & NegativeBinomial

Counting data are positive integer. Missing data are indicated by a `?`. Partial data can be provided through intervals denoted by
`[a:b]` where `a` and `b` are positive integers (`b` can be `+inf`).

#### JSON counting data

```json
{
    "varCount1": ["2", "3", "?", "4", "[2:4]", "[4:+inf]", "1"]
}
```

#### R counting data

```r
data <- list(
  varCount1 = c("2", "3", "?", "4", "[2:4]", "[4:+inf]", "1")
)

data <- data.frame(
  varCount1 = c("2", "3", "?", "4", "[2:4]", "[4:+inf]", "1")
)

data <- matrix(c("2", "3", "?", "4", "[2:4]", "[4:+inf]", "1"), ncol = 1, dimnames = list(NULL, c("varCount1")))
```

### Categorical Data: Multinomial

Modalities must be consecutive integers with 1 as minimal value. Missing data are indicated by a `?`.
For partial data, a list of possible values can be provided by `{a_1,...,a_j}`, where `a_i` denotes a modality.

Categorical data before formatting:

| varCateg1          | varCateg2       |
| ------------------ | --------------- |
| married            | large           |
| single             | small           |
| status unknown     | medium          |
| divorced           | small or medium |
| divorced or single | large           |

after formatting:

#### JSON categorical data

```json
{
    "varCat1": ["1", "2", "?", "3", "{2,3}"],
    "varCat2": ["3", "1", "2", "{1,2}", "3"]
}
```

#### R categorical data

```r
data <- list(
  varCat1 = c("1", "2", "?", "3", "{2,3}"),
  varCat2 = c("3", "1", "2", "{1,2}", "3")
)

data <- data.frame(
  varCat1 = c("1", "2", "?", "3", "{2,3}"),
  varCat2 = c("3", "1", "2", "{1,2}", "3")
)

data <- matrix(c("1", "2", "?", "3", "{2,3}",
                 "3", "1", "2", "{1,2}", "3"), ncol = 2, dimnames = list(NULL, c("varCat1", "varCat2")))
```

### Rank data

The format of a rank is: `o_1,..., o_j` where `o_1` is an integer corresponding to the the number of the object ranked in 1st position.
For example: `4,2,1,3` means that the fourth object is ranked first then the second object is in second position and so on.
Missing data can be specified by replacing and object by a `?` or a list of potential object, for example: `4, {2 3}, {2 1}, ?` means that
the object ranked in second position is either the object number 2 or the object number 3, then the object ranked in third position is either the object 2 or 1 and the last one can be anything.
A totally missing rank is specified by a sequence of `?` separated by commas, e.g. `?,?,?,?` for a totally missing rank of length 4.

#### JSON rank data

```json
{
    "varRank1": ["1,2,3,4", "2,1,3,4", "?,?,?,?", "4,{2,3},{1,3},{1,2}", "2,{1,3},4,{1,3}"]
}
```

#### R rank data

```r
data <- list(
  varRank1 = c("1,2,3,4", "2,1,3,4", "?,?,?,?", "4,{2,3},{1,3},{1,2}", "2,{1,3},4,{1,3}")
)

data <- data.frame(
  varRank1 = c("1,2,3,4", "2,1,3,4", "?,?,?,?", "4,{2,3},{1,3},{1,2}", "2,{1,3},4,{1,3}")
)

data <- matrix(c("1,2,3,4", "2,1,3,4", "?,?,?,?", "4,{2,3},{1,3},{1,2}", "2,{1,3},4,{1,3}"), ncol = 1, dimnames = list(NULL, c("varRank1")))
```

### Functional data: Func_CS & Func_SharedAlpha_CS

The format of a functional data is: `time_1:value_1,..., time_j:value_j`. Between individuals, functional data can have different length and different time values.
In the case of a functional model, `nSub: S, nCoeff: C` must be indicated in the third row of the descriptor file.
*S* is the number of subregressions in a functional data and *C* the number of coefficients of each regression (2 = linear, 3 = quadratic, ...). Totally missing data are not supported. Time points with missing values must not be included.

For example if you have a time vector `1 2 3 4 5 6 8 12` and a value vector `0.8 0.6 0.88 0.42 0.62 0.75 0.72 0.66` for one individual, the MixtComp format is `1:0.8,2:0.6,3:0.88,4:0.42,5:0.62,6:0.75,8:0.72,12:0.66`.

#### JSON functional data

```json
{
    "varFunc1": ["1:0.8,2:0.6,3:0.88,4:0.42,5:0.62,6:0.75,8:0.72,12:0.66", "1:0.5,2:0.4,4:0.32", "3:-0.8,5:-0.6,6:-0.88,8:-0.42,10:0.62"]
}
```

#### R functional data

```r
data <- list(
  varFunc1 = c("1:0.8,2:0.6,3:0.88,4:0.42,5:0.62,6:0.75,8:0.72,12:0.66", "1:0.5,2:0.4,4:0.32", "3:-0.8,5:-0.6,6:-0.88,8:-0.42,10:0.62")
)

data <- data.frame(
  varFunc1 = c("1:0.8,2:0.6,3:0.88,4:0.42,5:0.62,6:0.75,8:0.72,12:0.66", "1:0.5,2:0.4,4:0.32", "3:-0.8,5:-0.6,6:-0.88,8:-0.42,10:0.62")
)

data <- matrix(c("1:0.8,2:0.6,3:0.88,4:0.42,5:0.62,6:0.75,8:0.72,12:0.66", "1:0.5,2:0.4,4:0.32", "3:-0.8,5:-0.6,6:-0.88,8:-0.42,10:0.62"), ncol = 1, dimnames = list(NULL, c("varFunc1")))
```

#### JSON functional descriptor

```json
{
    "varName1": {
        "type": "Func_CS",
        "paramStr": "nSub: 3, nCoeff: 2"
    }
}
```

#### R functional descriptor

```r
desc <- list(varName1 = list(type = "Func_CS", paramStr = "nSub: 3, nCoeff: 2"))
```

### Missing data summary

|                         | Multinomial   | Gaussian   | Poisson    | NegativeBinomial | Weibull    |     Rank_ISR       | Func_CS |  LatentClass  |
| ----------------------- | :-----------: | :--------: | :--------: | :--------------: | :--------: | :----------------: | :-----: | :-----------: |
| Completely missing      | `?`           |  `?`       |  `?`       | `?`              | `?`        |  `?,?,?,?`         |         | `?`           |
| Finite number of values | `{a,b,c}`     |            |            |                  |            |  `4,{1 2},3,{1 2}` |         | `{a,b,c}`     |
| Bounded interval        |               |  `[a:b]`   | `[a:b]`    | `[a:b]`          | `[a:b]`    |                    |         |               |
| Right bounded interval  |               | `[-inf:b]` |            |                  |            |                    |         |               |
| Left bounded interval   |               | `[a:+inf]` | `[a:+inf]` | `[a:+inf]`       | `[a:+inf]` |                    |         |               |

### (Semi-)Supervised clustering

To perform a (semi-)supervised clustering, user can add a variable named `z_class` (with eventually some missing values) with `"LatentClass"` as model.
Missing data are indicated by a `?`. For partial data, a list of possible values can be provided by `{a_1,...,a_j}`, where `a_i` denotes a class number.

#### JSON (semi-)Supervised clustering

```json
{
    "varGauss1": ["2.1", "-0.26", "?", "[0.56:1.28]", "1.21", "[-inf:-0.11]", "[-1.65:+inf]"],
    "z_class" : ["1", "1", "{1,3}", "3", "?", "2", "1"]
}
```

```json
{
    "varGauss1": {
        "type": "Gaussian",
        "paramStr": ""
    },
    "z_class": {
        "type": "LatentClass",
        "paramStr": ""
    }
}
```

#### R (semi-)Supervised clustering

```r
data <- list(
  varGauss1 = c("2.1", "-0.26", "?", "[0.56:1.28]", "1.21", "[-inf:-0.11]", "[-1.65:+inf]"),
  z_class = c("1", "1", "{1,3}", "3", "?", "2", "1")
)

data <- data.frame(
  varGauss1 = c("2.1", "-0.26", "?", "[0.56:1.28]", "1.21", "[-inf:-0.11]", "[-1.65:+inf]"),
  z_class = c("1", "1", "{1,3}", "3", "?", "2", "1")
)

data <- matrix(c("2.1", "-0.26", "?", "[0.56:1.28]", "1.21", "[-inf:-0.11]", "[-1.65:+inf]",
                 "1", "1", "{1,3}", "3", "?", "2", "1"), ncol = 2, dimnames = list(NULL, c("varGauss1", "z_class")))
```

```r
desc <- list(varGauss1 = list(type = "Gaussian", paramStr = ""),
             z_class = list(type = "LatentClass", paramStr = ""))
```
