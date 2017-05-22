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
    dans param\$categorical1\$NumericalParam\$stat est sous format de liste et non de matrice
    dans param\$categorical1\$NumericalParam\$log est sous format de liste et non de matrice
- Poisson
    dans param\$poisson1\$NumericalParam\$log est sous format de liste et non de matrice
    dans param\$poisson1\$NumericalParam\$stat est sous format de liste et non de matrice
- Ordinal
    données complétées décalées tt de 1
    dans param\$Ordinal1\$muPi\$log est sous format de liste et non de matrice
    dans param\$Ordinal1\$muPi\$stat est sous format de liste et non de matrice
- z_class
    dans param\$z_class\$pi\$stat est sous format de liste et non de matrice
    dans param\$z_class\$pi\$log est sous format de liste et non de matrice
- gaussian
    dans param\$gaussian1\$NumericalParam\$stat est sous format de liste et non de matrice
    dans param\$gaussian1\$NumericalParam\$log est sous format de liste et non de matrice
