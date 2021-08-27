# PyMixtComp

## Requirements

*pyMixtComp* requires python and the *numpy* package. See [MixtComp's README](../MixtComp/README.md) for MixtComp's requirements.

``` bash
sudo apt install python3-pip
pip3 install numpy
```

It requires [Boost.Python](https://www.boost.org/doc/libs/1_72_0/libs/python/doc/html/index.html) library.

``` bash
brew install boost --with-python
```

## Compile PyMixtComp

Run `./buildDebug.sh` (resp. `./buildRelease.sh`) to compile MixtComp in Debug (resp. Release) mode.
Run `./utestDebug.sh` (resp. `./utestRelease.sh`) to run unit tests in Debug (resp. Release) mode.
To clean the debug and release folders, run `./clean.sh`.

## Python library

After compiling, *pyMixtCompBridge* python library can be found at the following location: `debug/lib/pyMixtCompBridge.so` and `release/lib/pyMixtCompBridge.so`.

## Using Python library

In Python console, run:

``` python
import pyMixtCompBridge
```

It contains the *pmc* function that performs learn and predict. The function uses dictionaries for the different parameters.

*pmc* requires four parameters: four input dictionaries (algo, data, desc and res, see [Data Format](../MixtComp/docs/dataFormat.md)). To perform learning, the *res* parameter must be an empty dictionary. Ine predict mode, *res* is the output dictionary of a *pmc* run. To have more information about the output format, read the associated [documentation](../MixtComp/docs/objectOutput.md).

## Examples

See the file [here](python/pyMixtComp.py).
