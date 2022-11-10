# pyMixtComp

Python package of the C++ library MixtComp.
Mixture models with heterogeneous data sets and partially missing data management.

## Installation

### docker

In the root directoy of the project (where the Dockerfile is located), run:

Build image:
``` bash
make build
```
Start a jupyter server with a notebook
``` bash
make notebook
```

### Compilation

First, compile the C++ library:

``` bash
./build.sh
```

Then, install the requirements:

``` bash
pip3 install -r requirements.txt
```

And finally the python package:

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
pytest-3 .
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
