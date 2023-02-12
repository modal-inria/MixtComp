# Graph

Inputs and outputs are given as Graph objects. MixtComp methods are templated with this Graph structure. Three Graph structures exists to read and save:

- json objects (*JSONGraph* class, cf. [JMixtComp](../../JMixtComp/src/lib/JSONGraph.h))
- R objects (*RGraph* class, cf. [RMixtCompIO](../../RMixtCompIO/src/RGraph.h))
- python objects (*PyGraph* class, cf. [pyMixtComp](../../pyMixtComp/src/lib/PyGraph.h))

The different *Graph* classes must implement the following method:

- `void getSubGraph(const std::vector<std::string>& path, Graph& j) const;`: get the Graph at the given path. It modifies the `j` argument.
- `template<typename Type> void add_payload(const std::vector<std::string>& path, const std::string& name, const Type& p);`: add an object in the Graph and create the complete path to it if it does not exist yet.
- `void addSubGraph(const std::vector<std::string>& path, const std::string& name, const Graph& p);`: add an other Graph in the Graph and create the complete path to it if it does not exist yet.
- `template<typename Type> Type get_payload(const std::vector<std::string>& path, const std::string& name) const;`: get the element named `name` in `path`
- `template<typename Type> void get_payload(const std::vector<std::string>& path, const std::string& name, Type& p);`: get the element named `name` in `path`. This version modifies the `p` argument.
- `bool exist_payload(const std::vector<std::string>& path, const std::string& name) const;`: test if an element named `name` exists in `path`.
- `void name_payload(const std::vector<std::string>& path, std::list<std::string>& l) const;`: get the names of all the elements in `path` It modifies the `l` argument.

with `path` a vector containing the path of the element to reach or save an element (e.g. ["variable", "param", "var1", "stat"] the element in the following path: variable/param/var1/stat). `Type` can be `int`, `double`, `bool`, `vector<T>`, `vector<vector<T> >`, `NamedVector<T>`, `NamedMatrix<T>`. `NamedMatrix<T>` is a struct containing an Eigen Matrix `mat_` and two `vector<string>` named `colnames_` and `rownames_`. `NamedVector<T>` is a struct containing an Eigen Matrix with 1 column `vec_` and a `vector<string>` named `rownames_`  (see [NamedAlgebra](../src/lib/IO/NamedAlgebra.h) and [LinAlg](../src/lib/LinAlg/LinAlg.h) for mode details about `NamedVector` and `NamedMatrix`).

## Structure of input Graphs

Three Graphs for learning (and four for prediction)  (see [file](./dataFormat.md) for mdoe details):

- algo
  - nClass
  - nInd
  - nbBurnInIter
  - nbIter
  - nbGibbsBurnInIter
  - nbGibbsIter
  - nInitPerClass
  - nSemTry
  - ratioStableCriterion
  - nStableCriterion
  - confidenceLevel
  - mode ("learn" or "predict")

- data
  - var1 (array of string)
  - var2
  - ...

- desc
  - var1
    - type
    - paramStr
  - var2
  - ...

- param (output Graph of a MixtComp run. Only for prediction, see below)

## Structure of output Graph

An unique Graph containing: (see [file](./objectOutput.md) for more details)

- algo (copy of algo input)
- mixture
- variable
  - type
    - var1: type of var1
    - var2: type of var2
  - data
    - var1
      - completed
      - stat (to be implemented, except for LatentClass -> immediate implementation)
    - var2
    - ...
  - param
    - var1 (case 1: simple model)
      - log
      - stat
      - paramStr
    - var2 (case 2: functional)
      - alpha
        - log
        - stat
      - beta
        - log
        - stat
      - sd
        - log
        - stat
      - paramStr
    - var3 (case 2: rank)
      - mu
        - log
        - stat
      - pi
        - log
        - stat
      - paramStr
    - ...
