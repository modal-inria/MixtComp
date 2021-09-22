# JMixtComp

## Compile JMixtComp

Run `./build.sh` to compile MixtComp in Debug mode. Run `./build.sh Release` or `./build.sh Coverage` to build in release mode or for code coverage.  The build is performed in a build folder.
Run `./utest.sh` to run unit tests .
To clean the build folder, run `./clean.sh`.

## Executables

After compiling, *jmc* executables can be found at the following location: `build/lib/jmc`. It allows to run MixtComp in command line with json as format for input and output files.

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

Datasets can be found in the [data folder](data) and can be used with jmc by running the [runTest.sh](runTest.sh) file.
