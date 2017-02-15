# discussion
- deux types de dégénérescence, vraisemblance bornée ou pas
- dans l'algo, pas de bornes, rajouter les bornes avant l'export de paramètres
- si en prédiction, avec bornes partout, la variable devient muette
- Est-ce qu'il ne vaudrait pas mieux initialiser les paramètres en tirant des individus, pour aller plus vite
- Initialiser en générant une liste de z
    - soit le modèle est capable d'estimer à partir d'un individu, et c'est le centre de la classes
    - soit il peut pas (multinomial), et dans ce cas, il utilise toute la liste pour initialiser comme actuellement
    - au pire des cas on est dans le cas actuel, et au mieux, on explore mieux l'espace
- Si l'algo est plus rapide, on peut intégrer les initalisations multiples, et renvoyer à l'utilisateur seulement le meilleur.
- Sortir les vraisemblances individuelles des individus, pour éviter d'avoir à lancer des prédictions à un individu

# Bugs

- Functional model seem to crash when edge authorized option is activated for the estimators
    - They do not support biased estimators and since the Gibbs is deactivated, null variance or similar errors could happen
- parameterEdgeAuthorized limits the estimation by excluding the border of the parameter space
	- if a class is empty, its proportion will be estimated at 1e-8. BUT the estimation of the classes will not work as there are no observations. Crashes are to be expected.
- Uncomment and correct SimpleMixture utest

# Features

- Switch to Gitlab
- Put in place continuous integration
    - Add compilation flag to set random seed, to allow reproducibility of stochastic tests

# Estimation problem
- keep the test for degenerescency
- remove the Gibbs
    - how will the Ordinal model work without Gibbs ?
    - it will not work.
- remove parameterEdgeAuthorized
    - it "worked" for problem of null variance and such
    - it never worked for empty classes
    - remove it altogether ?
- launch multiple run if a degenerescency is detected
- SEM will degenerate given enough time, so if the data is "correct", this should be rare, and fixed by relaunching
- if degenerescency is systematic, the model is not correct and should be dropped. Running a Gibbs over a long time period will not solve anything.

# Model

- Missing data in support in Functional Model
- Ordinal model
    - clarify if can be used with EdgeAuthorized or not. It seems it cannot. Test with generated data.
- remove regex in data parsing. Will be faster but less tolerant to data format.

# R

- Export data in functional Model
- Refactor RMixtComp package files (R subdirectory)
- Create a skeleton package which only uses the MixtComp binary, to ease transfer to the industrial clients

# Json

- Output rows names as a vector to easily recreate arrays
- In input, separate
    - strategy and number of classes
    - descriptor
    - this way you can edit the strategy and model parameters without editing the data file
- in predict mode, can't read file path with : or ~ 
- exportVals function for functional data

# Visualization and post-processing

# Long term

- co-clustering
- parallel Spark
