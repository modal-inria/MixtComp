# JMixtComp

## Compile JMixtComp

Run `./builDebug.sh` (resp. `./builRelease.sh`) to compile MixtComp in Debug (resp. Release) mode.
Run `./utestDebug.sh` (resp. `./utestRelease.sh`) to run unit tests in Debug (resp. Release) mode.
To clean the debug and release folders, run `./clean.sh`.

## Executables

After compiling, *jmc* executables can be found at the following location: `debug/lib/jmc` and `release/lib/jmc`. They allow to run MixtComp in command line with json as format for input and output files.

## Running jmc

In learning mode, *jmc* requires four parameters: three input files (algo, data and desc, see [Data Format](../MixtComp/docs/dataFormat.md)) and the name of the output file. To have more information about the output format, read the associated [documentation](../MixtComp/docs/objectOutput.md).

```bash
jmc algo.json data.json desc.json resLearn.json
```

In predict mode, *jmc* requires five parameters: four input files (algo, data and desc, see [Data Format](../MixtComp/docs/dataFormat.md) and a result of a learning run of *jmc*) and the name of the output file.

```bash
jmc algo.json data.json desc.json resLearn.json resPredict.json
```

## Examples

Datasets can be found in the [data folder](data) and command lines are avaible in the [runTestDebug.sh](runTestDebug.sh) and [runTestRelease.sh](runTestRelease.sh) files.
