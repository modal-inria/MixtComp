def create_algo(object, X, mode):
    algo = {
        "nbBurnInIter": object.n_burn_in_iter,
        "nbIter": object.n_iter,
        "nbGibbsBurnInIter": object.n_gibbs_burn_in_iter,
        "nbGibbsIter": object.n_gibbs_iter,
        "nInitPerClass": object.n_init_per_class,
        "nSemTry": object.n_sem_try,
        "confidenceLevel": object.confidence_level,
        "ratioStableCriterion": object.ratio_stable_criterion,
        "nStableCriterion": object.n_stable_criterion
    }
    algo["mode"] = mode
    algo["nInd"] = len(X[list(X.keys())[0]])
    algo["nClass"] = object.n_component

    return algo
