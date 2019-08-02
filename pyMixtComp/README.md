# PyMixtComp

## Requirements

pyMixtComp requires python and the *numpy* package. See [MixtComp's README](../MixtComp/README.md) for MixtComp's requirements.

```
sudo apt install python-pip
pip install numpy
```

## Compile PyMixtComp

Run `./builDebug.sh` (resp. `./builRelease.sh`) to compile MixtComp in Debug (resp. Release) mode.
Run `./utestDebug.sh` (resp. `./utestRelease.sh`) to run unit tests in Debug (resp. Release) mode.
To clean the debug and release folders, run `./clean.sh`.

## Python library

After compiling, *pyMixtComp* python library can be found at the following location: `debug/lib/pyMixtComp.so` and `release/lib/pyMixtComp.so`. 

## Using Python library

In Python console, run:

```
import pyMixtComp
```

It contains the *pmc* function that performs learn and predict. The function uses dictionaries for the different parameters.

*pmc* requires four parameters: four input dictionaries (algo, data, desc and res, see [Data Format](../MixtComp/docs/dataFormat.md)). To perform learning, the *res* parameter must be an empty dictionary. Ine predict mode, *res* is the output dcitionary of a *pmc* run. To have more information about the output format, read the associated [documentation](../MixtComp/docs/objectOutput.md).

## Examples

See the file [here](python/pyMixtComp.py).
