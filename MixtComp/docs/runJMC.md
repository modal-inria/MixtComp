# Running JMixtComp

## Executables

After [compiling MixtComp](../README.md), *jmc* executables can be found at the following location: `debug/json/jmc` and `release/json/jmc`. They allow to run MixtComp in command line with json as format for input and output files.


## Running jmc

In learning mode, *jmc* requires four parameters: three input files (algo, data and desc, see [Data Format](dataFormat.md)) and the name of the output file. To have more information about the output format, read the associated [documentation](objectOutput.md).
```
jmc algo.json data.json desc.json resLearn.json
```

In predict mode, *jmc* requires five parameters: four input files (algo, data and desc, see [Data Format](dataFormat.md) and a result of a learning run of *jmc*) and the name of the output file.
```
jmc algo.json data.json desc.json resLearn.json resPredict.json
```


## Examples

Datasets can be found in the [data folder](../data) and command lines are avaible in the [runTestDebug.sh](../runTestDebug.sh) and [runTestRelease.sh](../runTestRelease.sh) files.
