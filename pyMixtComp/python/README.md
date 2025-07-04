# pyMixtComp

Python package of the C++ library MixtComp.
Mixture models with heterogeneous data sets and partially missing data management.

## Installation

First, compile the C++ library:

``` bash
./build.sh
```

The python package uses [poetry](https://python-poetry.org/docs/#installation) as build system.
Install pyMixtComp using pip:

``` bash
pip3 install .
```

## Run tests

Install pytest:

``` bash
pip3 install pytest
```

Run tests:

``` bash
pytest .
```

## Examples

``` python
import matplotlib.pyplot as plt
from pyMixtComp import MixtComp
from pyMixtComp.data import load_prostate
data, model = load_prostate()
data.head()
```

Prostate data are mixed data (real and categorical). All variables are stored as object, "?" indicates missing values.
And the model dict indicates which model used for every variable.

``` python
model
mod = MixtComp(n_components=2, n_init=5)
mod.fit(data[:-50], model)
```

Plot the discriminative power of variables to find the variables that participate the most at the clustering task.

``` python
mod.plot_discriminative_power_variable()
plt.show()
```

SG and AP are the most discriminant variables.

``` python
mod.get_param("SG")
```

Predict the cluster of the 50 last data

``` python
mod.predict(data[-50:], model)
```
