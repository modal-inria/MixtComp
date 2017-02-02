# Bugs

- Functional model seem to crash when edge authorized option is activated for the estimators
    - They do not support biased estimators and since the Gibbs is deactivated, null variance or similar errors could happen
- parameterEdgeAuthorized limits the estimation by excluding the border of the parameter space
	- if a class is empty, its proportion will be estimated at 1e-8. BUT the estimation of the classes will not work as there are no observations. Crashes are to be expected.

# Model

- Missing data in support in Functional Model
- Ordinal model
    - clarify if can be used with EdgeAuthorized or not. It seems it cannot. Test with generated data.

# IO

- remove regex in data parsing. Will be faster but less tolerant to data format.

## R

- Export data in functional Model

## Json

# Visualization and post-processing

# Long term

- co-clustering
- parall Spark
