- Output rows names as a vector to easily recreate arrays
- In input, separate
    - strategy and number of classes
    - descriptor
    - this way you can edit the strategy and model parameters without editing the data file
- in predict mode, can't read file path with : or ~
- exportVals function for functional data
- delta manquant dans $mixture
- IDClass est transposée par rapport à la sortie de RMixtComp
- Rank
    Problème de lecture des données de rangs (le même jeu de données marche avec RMixtComp)
- Functional
    Problème de lecture des données fonctionnelles (le même jeu de données marche avec RMixtComp)
- Categorical
    données complété stat : retourne 0.52 0.52 au lieu d'une liste [[1]] 2 [[2]] 0.52

